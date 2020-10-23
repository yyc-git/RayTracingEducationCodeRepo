module PathTracePass = {
  let _traceRay = (ray, sceneInstancesContainer) =>
    if (!isValid(ray)) {
      {isValid: false};
    } else if (isHit(ray, sceneInstancesContainer)) {
      {
        //hit

        isHit: true,
        radiance: evalByBRDFOrBSDF(ray, sceneBufferData),
        hitPosition: compute(ray, sceneBufferData),
        scatterDirection: sample(ray, sceneBufferData),
      };
    } else {
      {
        //miss

        isHit: false,
        radiance: getFromBackground(ray),
      };
    };

  let execute = () => {
    screenAllPixels->reduce(
      (pixelBuffer, pixelIndex) => {
        let pixelColor =
          sampleCount->reduce(
            (pixelColor, _) => {
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
                      //sample ray
                      generateRay(
                        result.hitPosition,
                        result.scatterDirection,
                      ),
                    );
                  },
                  (
                    false,
                    vec3(0.0, 0.0, 0.0),
                    //generate ray from camera to pixel
                    generateRay(cameraPosition, cameraToPixelDirection),
                  ),
                );

              pixelColor + radiance;
            },
            vec3(0.0, 0.0, 0.0),
          );

        pixelBuffer[pixelIndex] = pixelColor;
      },
      emptyPixelBufferUploadFromCPU(),
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

module PostEffectPass = {
  let execute = () => {
    screenAllPixels->forEach(outputPixelColor => {
      outputPixelColor->gammaCorrection->output
    });
  };
};

let inOneFrame = () => {
  PathTracePass.execute();
  AccumulationPass.execute();
  PostEffectPass.execute();
};

inOneFrame();
