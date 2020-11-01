let rec shade = (p, wo) => {
  let P_RR = defineProbability();
  let ksi = random();

  if (ksi > P_RR) {
    (0.0, 0.0, 0.0);
  } else {
    //在半球面面随机均匀采样1个方向
    let wi = sample(ray, sceneInstancesContainer);

    let result = _traceRay(generateSampleRay(p, wi));

    if (isHitLight(result)) {
      L_i * f_r * cosine / pdf(wi) / P_RR;
    } else if (isHitObject(result)) {
      shade(result.hitPosition, wi) * f_r * cosine / pdf(wi) / P_RR;
    } else {
      backgroudColor / P_RR;
    };
  };
};
