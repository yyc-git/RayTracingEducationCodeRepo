/*! “->”是函数组合的操作符。假设a是一个Primitive的数据（如整数、浮点数、字符串），func是一个函数。对于“a->func”，意思是将a作为func函数的第一个参数。
 * e.g.
 *  1 -> func 相当于func(1)
 *  1 -> func(2) 相当于func(1, 2)
 *  func1() -> func2(2) 相当于func2(func1(), 2)
 */

//r,g,b,a
type pixelColor = (float, float, float, float);

type pixelData = {
  // 像素的序号
  pixelIndex: int,
};

type screenAllPixels = list(pixelData);

// 从每帧的采样数而生成的一个列表集合，用于遍历操作（如reduce）
// e.g. 假设每帧的采样数是32，则samples = [0,1,2,...,31];
type samples = list(int);

// 从射线反弹次数而生成的一个列表集合，用于遍历操作（如reduce）
// e.g. 假设反弹次数是5，则boundces = [0,1,2,3,4,5];
type boundces = list(int);

// PixelBuffer是Storage Buffer，由CPU创建并传递到GPU。
// PixelBuffer负责存储路径追踪pass计算的每个像素的颜色值
type emptyPixelBufferUploadFromCPU = list(pixelColor);

/*! 对列表中的每个元素执行reducer函数(升序执行)，将其结果汇总为单个返回值
 * e.g.
 * //sum == 0+1+2
 * let sum = [0,1,2] -> reduce(0, (sum, value) => sum + value);
  */
type reduce = (list('value), 'acc, ('acc, 'value) => 'acc) => 'acc;

/*! 对列表的每个元素执行一次给定的函数
 * e.g.
 * //分别打印0,1,2
 * [0,1,2] -> forEach(value => Js.log(value));
 **/
type forEach = (list('value), 'value => unit);
