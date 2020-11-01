module PathTracePass = {
  let _traceRay = (ray, sceneInstancesContainer) =>
    intersect(ray, sceneInstancesContainer);

  let rec shade = (p, wo) => {
    //在半球面面随机均匀采样1个方向
    let wi = sample(ray, sceneInstancesContainer);

    let result = _traceRay(generateSampleRay(p, wi));

    if (isHitLight(result)) {
      L_i * f_r * cosine / pdf(wi);
    } else if (isHitObject(result)) {
      shade(result.hitPosition, wi) * f_r * cosine / pdf(wi);
    } else {
      backgroudColor;
    };
  };

  let execute = () => {
    screenAllPixels->reduce(
      pixelBuffer,
      ({pixelIndex}) => {
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
        //将N个相邻帧的像素值相加，取平均值
        (addNPixelColorFromPixelBuffer(pixelBuffer) / totalSampleCount)
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
