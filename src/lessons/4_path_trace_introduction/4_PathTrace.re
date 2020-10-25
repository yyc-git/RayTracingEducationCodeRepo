module PathTracePass = {
  let _traceRay = (ray, sceneInstancesContainer) =>
    if (!isValid(ray)) {
      {isValid: false};
    } else {
      let result = intersect(ray, sceneInstancesContainer);

      if (result.isHit) {
        {
          //相交

          isValid: true,
          isHit: true,
          radiance:
            shadingByBRDF(ray, result.hitShadingData, sceneBufferData)
            / computePDF(sceneBufferData),
          hitPosition,
          scatterDirection: sample(ray, sceneBufferData),
        };
      } else {
        {
          //丢失

          isValid: true,
          isHit: false,
          radiance: shadingFromBackground(ray),
        };
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

              //退出bounce
              if (!result.isHit || !result.isValid) {
                (true, radiance, ray);
              };

              (
                radiance,
                //根据采样方向，生成新的射线
                generateSampleRay(
                  result.hitPosition,
                  result.scatterDirection,
                ),
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
        (addAllPixelColorFromPixelBuffer(pixelBuffer) / totalSampleCount)
        ->output
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
