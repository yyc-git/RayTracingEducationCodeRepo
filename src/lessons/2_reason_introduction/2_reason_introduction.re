type animal = 
| Cat
| Dog;

let a = Cat;

let interact = (animal) => {
  switch (animal) {
  | Cat => "pet"
  | Dog => "throw ball"
  };
};

//"pet"
let c = interact(a);


// var a = null;

let a:option(int) = None;

let a = Some(1);




type t = A | B | C;

let x:t = B;

//"one"
let y =
  switch (x) {
  | A => "zero"
  | B =>
    "one";
  | C => "two"
  };