module PathTracePass = {
  let _traceRay = (ray, sceneInstancesContainer) =>
    intersect(ray, sceneInstancesContainer);

  let rec shade = (P_RR, p, p', p'', wo) => {
    let L_dir =
      if (isVisible(p', p'')) {
        L_e(p', p'') * f_r(p, p', p'') * cosθ * cosθ' / |p'' - p'|^2 / pdf(p'')
      } else {
        0.0;
      };

    let ksi = random();

    let L_indir =
      if (ksi > P_RR) {
        (0.0, 0.0, 0.0);
      } else {
        //在半球面随机均匀采样1个方向
        let wi = sample(generateCameraRay(p', wo), sceneInstancesContainer);

        let result = _traceRay(generateSampleRay(p, wi));

        //这里不需要再对光源进行着色计算了

        if (isHitObject(result)) {
          shade(P_RR, p', result.hitPosition, p'', -wi) * f_r(p', wi, wo) * cosθ / pdf(wi) / P_RR;
        } else {
          backgroudColor / P_RR;
        };
      };

    L_e(p', wo) + L_dir + L_indir
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
          radiance = shade(defineProbability(), cameraPosition, result.hitPosition, lightPosition, cameraToPixelDirection);
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
