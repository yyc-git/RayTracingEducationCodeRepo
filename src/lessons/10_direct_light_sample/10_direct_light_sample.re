module PathTracePass = {
  let _traceRay = (ray, sceneInstancesContainer) =>
    intersect(ray, sceneInstancesContainer);

  //P_RR = defineProbability();
  let rec shade = (P_RR, p, wo) => {
    let L_dir =
      if (isVisible(result.hitPosition, p)) {
        L_i * f_r * cosθ * cosθ' / |result.hitPosition, p|^2 / pdf_light
      } else {
        0.0;
      };

    let ksi = random();

    let L_indir =
      if (ksi > P_RR) {
        (0.0, 0.0, 0.0);
      } else {
        //在半球面随机均匀采样1个方向
        let wi = sample(ray, sceneInstancesContainer);

        let result = _traceRay(generateSampleRay(p, wi));

        if (isHitObject(result)) {
          shade(result.hitPosition, -wi) * f_r * cosθ / pdf(wi) / P_RR;
        } else {
          backgroudColor / P_RR;
        };
      };

    L_dir + L_indir;
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
