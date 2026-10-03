# JavaScript Journal

The main hurdle in the past for me when using JavaScript for AoC was figuring out how to read piped input from the command line. I had taken NodeSchool courses when I first started out with JavaScript but at the time I had no clue what 'piping' and 'streams' were. Now I get it.

My early solution is the following boilerplate:
```js
const readline = require('node:readline');
const rl = readline.createInterface(process.stdin);

rl.on('line', (input) => {
    // do stuff
});

rl.on('close', () => {
    // report results
});
```

### A little while later..

So apparently there's "CommonJS" and "ECMA Script Modules" as two branches of
JavaScript..? I vaguely remember CommonJS as a thing but I wasn't doing big projects so I didn't really need "require" and stuff.

For the fourth day I swapped to the ESM way of importing libraries. Good to know both, I guess.

There was some other ECMA Script 2015 stuff that I had a chance to learn more about, namely the spread operator and destructuring.

The fourth day required the changing of characters in a string, something that 
JavaScript is surprisingly strict about NOT allowing. You need to explicitly convert a string to an array of chars before you can manipulate chars.

```js
let s = "Welcome back!";

// don't do
s[8] = 'h';                 // bad!

// do
let a = [...s];             // spread the love into an array
a[8] = 'h';

console.log(a.join(''));    // Welcome hack!
```

Destructuring revealed its soft, sensitive underbelly to me, specifically when returning multiple values from a function. I've used it in other languages and had little to no issues. In this example I kept getting `NaN` and `undefined` return values. Apparently the receiving variables need to have the exact same names as the sending variables at the end of a function.

The issue arose because I was returning into an already defined variable and a new one. Instead of creating two new variables and copying one into the named target, I needed to do assignment wrapped in parenthesis.

```js
function twoValues {
    let one = 1;
    let two = 2;
    return { one, two };
}

// don't do
let one = 0;
let { newOne, two } = twoValues();  // newOne -> undefined
one = newOne;                       // one -> NaN

// do
let one = 0;
let two = 0;
({ one, two } = twoValues());       // one -> 1, two -> 2
```

I dunno, maybe it was obvious. Look at the solution, the real problem was, predictably, naming things.
