open 4_Explain;

module PathTracePass = {
  let _traceRay = (ray, sceneInstancesContainer) =>
    if (!isValid(ray)) {
      {isValid: false};
    } else if (isHit(ray, sceneInstancesContainer)) {
      {
        //相交

        isHit: true,
        //now not consider about BSDF
        radiance:
          shadingByBRDF(ray, sceneBufferData) / computePDF(sceneBufferData),
        hitPosition,
        scatterDirection: sample(ray, sceneBufferData),
      };
    } else {
      {
        //丢失

        isHit: false,
        radiance: shadingFromBackground(ray),
      };
    };

  let execute = () => {
    screenAllPixels->reduce(
      (pixelBuffer, {pixelIndex}) => {
        let (_, radiance, _) =
          bounces->reduce(
            ((isBreak, radiance, ray), _) => {
              if (isBreak) {
                (isBreak, radiance, ray);
              };

              let result = _traceRay(ray, sceneInstancesContainer);
              radiance += result.radiance;

              //break
              if (!result.isHit || !result.isValid) {
                (true, radiance, ray);
              };

              (
                radiance,
                //根据采样方向，生成新的射线
                generateSampleRay(result.hitPosition, result.scatterDirection),
              );
            },
            (
              false,
              //r,g,b
              (0.0, 0.0, 0.0),
              generateCameraRay(cameraPosition, cameraToPixelDirection),
            ),
          );

        //像素颜色的alpha为1.0
        pixelBuffer[pixelIndex] = (radiance, 1.0);
      },
      emptyPixelBufferUploadFromCPU,
    );
  };
};

module AccumulationPass = {
  let execute = () => {
    screenAllPixels->forEach(
      _ => {
        (addAllPixelColorFromPixelBuffer(pixelBuffer) / totalSampleCount) -> output
      },
      (),
    );
  };
};

let inOneFrame = () => {
  PathTracePass.execute();
  AccumulationPass.execute();
};

inOneFrame();
