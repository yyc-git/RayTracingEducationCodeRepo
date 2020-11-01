module PathTracePass = {
  let _traceRay = (ray, sceneInstancesContainer) =>
    intersect(ray, sceneInstancesContainer);

  let shade = (p, wo) => {
    //在半球面面均匀采样N个方向
    let scatterDirections = sampleN(ray, sceneInstancesContainer);

    scatterDirections->reduce(
      L0,
      wi => {
        let result = _traceRay(generateSampleRay(p, wi));

        if (isHitLight(result)) {
          Lo += 1 / N * L_i * f_r * cosine / pdf(wi);
        };
      },
      (0.0, 0.0, 0.0),
    );
  };

  let execute = () => {
    screenAllPixels->forEach(({pixelIndex}) => {
      let radiance = (0.0, 0.0, 0.0);
      let result =
        _traceRay(
          generateCameraRay(cameraPosition, cameraToPixelDirection),
          sceneInstancesContainer,
        );

      if (result.isHit) {
        radiance = shade(result.hitPosition, cameraToPixelDirection);
      } else {
        radiance = backgroudColor;
      };

      //像素颜色的alpha为1.0
      output(radiance, 1.0);
    });
  };
};

let inOneFrame = () => {
  PathTracePass.execute();
};

inOneFrame();
