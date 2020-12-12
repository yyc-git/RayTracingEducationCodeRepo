let rec shade = (p, wo) => {
  //在半球面均匀采样N个方向
  let scatterDirections = sampleN(ray, sceneInstancesContainer);

  scatterDirections->reduce(
    L0,
    wi => {
      let result = _traceRay(generateSampleRay(p, wi));

      if (isHitLight(result)) {
        Lo += 1 / N * L_i * f_r * cosine / pdf(wi);
      } else if (isHitObject(result)) {
        Lo += 1 / N * shade(result.hitPosition, wi) * f_r * cosine / pdf(wi);
      } else {
        Lo += 1 / N * backgroudColor;
      };
    },
    (0.0, 0.0, 0.0),
  );
};
