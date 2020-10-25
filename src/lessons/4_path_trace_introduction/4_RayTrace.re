module GBufferPass = {
  let execute = () => {
    screenAllPixels->reduce(
      (gbuffers, {pixelIndex}) => {
        gbuffers[pixelIndex] = {worldPosition, worldNormal, diffuseColor}
      },
      emptyGBuffersUploadFromCPU,
    );
  };
};

module RayTracePass = {
  let _directLightShading = (ray, hitShadingData, sceneBufferData) => {
    let radiance = (0.0, 0.0, 0.0);

    if (isShadow()) {
      radiance = (0.0, 0.0, 0.0);
    } else {
      radiance = shadingByBRDF(ray, hitShadingData, sceneBufferData);
    };
  };

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
            _directLightShading(ray, result.hitShadingData, sceneBufferData),
          hitPosition,
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

  //递归函数
  let rec _bounce =
          (radiance, isValid, isHit, sceneBufferData, sceneInstancesContainer) =>
    //退出bounce
    if (isStopBounce() || !isValid || !isHit) {
      radiance;
    } else {
      if (isReflect(materialFromSceneBufferData)) {
        let result = _traceRay(generateReflectRay(), sceneInstancesContainer);

        _bounce(
          radiance + reflectRatio * result.radiance,
          result.isValid,
          result.isHit,
          sceneBufferData,
          sceneInstancesContainer,
        );
      };
      if (isRefract(materialFromSceneBufferData)) {
        let result = _traceRay(generateRefractRay(), sceneInstancesContainer);

        _bounce(
          radiance + refractRatio * result.radiance,
          result.isValid,
          result.isHit,
          sceneBufferData,
          sceneInstancesContainer,
        );
      };
      if (isComputeAO(materialFromSceneBufferData)) {
        let result = _traceRay(generateAORay(), sceneInstancesContainer);

        _bounce(
          radiance + aoRatio * result.radiance,
          result.isValid,
          result.isHit,
          sceneBufferData,
          sceneInstancesContainer,
        );
      };
      if (isComputeGlobalIllumination(materialFromSceneBufferData)) {
        radiance += giRatio;
        let result = _traceRay(generateGIRay(), sceneInstancesContainer);

        _bounce(
          radiance + giRatio * result.radiance,
          result.isValid,
          result.isHit,
          sceneBufferData,
          sceneInstancesContainer,
        );
      };

      radiance;
    };

  let execute = () => {
    screenAllPixels->reduce(
      (gbuffers, pixelIndex) => {
        let radiance =
          _bounce(
            _directLightShading(
              generateCameraRay(cameraPosition, cameraToPixelDirection),
              getHitShadingDataFromGBuffer(gbuffers, pixelIndex),
              sceneBufferData,
            ),
            sceneBufferData,
            sceneInstancesContainer,
          );

        //像素颜色的alpha为1.0
        pixelBuffer[pixelIndex] = (radiance, 1.0);
      },
      emptyPixelBufferUploadFromCPU,
    );
  };
};

module DenoisePass = {
  let execute = () => {
    screenAllPixels->forEach(
      _ => {
        //取出多个相邻帧内的pixel color
        let pixelColors = getPixelColorsFromPixelBuffer(pixelBuffer);

        denoise(pixelColors, gbuffers)->output;
      },
      (),
    );
  };
};

let inOneFrame = () => {
  GBufferPass.execute();
  RayTracePass.execute();
  DenoisePass.execute();
};

inOneFrame();
