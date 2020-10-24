open 4_Explain;

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
  //递归函数
  let rec _traceRay = (ray) =>{
        let radiance = (0.0, 0.0, 0.0);

        //计算直接光照

        if (isShadow()) {
          radiance = (0.0, 0.0, 0.0);
        } else {
          radiance =
            shadingDirectLightFromGBuffer(gbuffers, pixelIndex, material);
        };

        if (isReflect(material)) {
          radiance += reflectRatio * _traceRay(generateReflectRay(), sceneInstancesContainer);
        };
        if (isRefract(material)) {
          radiance += refractRatio * _traceRay(generateRefractRay(), sceneInstancesContainer); 
        };
        if(isComputeAO(material)){
          radiance += aoRatio * _traceRay(generateAORay(), sceneInstancesContainer); 
        }
        if(isComputeGlobalIllumination(material)){
          radiance += giRatio * _traceRay(generateGIRay(), sceneInstancesContainer); 
        }

        radiance;
  };

  let execute = () => {
    screenAllPixels->reduce(
      (gbuffers, pixelIndex) => {
        let radiance = _traceRay(generateCameraRay(cameraPosition, cameraToPixelDirection));

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

        denoise(pixelColors, gbuffers) -> output
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
