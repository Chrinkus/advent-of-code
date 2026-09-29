# JavaScript Journal

The main hurdle in the past for me when using JavaScript for AoC was figuring out how to read piped input from the command line. I had taken NodeSchool courses when I first started out with JavaScript but at the time I had no clue what 'piping' and 'streams' were. Now I get it.

My early solution is the following boilerplate:
```
const readline = require('node:readline');
const rl = readline.createInterface(process.stdin);

rl.on('line', (input) => {
    // do stuff
});

rl.on('close', () => {
    // report results
});
```
