# JavaScript Journal

After starting the 2025 event in JavaScript I figured I'd try some of the older events as well. Apparently I was NOT in the mood for the 2023 event so I'm branching out here. Let's get some easy stars!

### Factory Functions

After writing C for so long I've gotten used to how easy structs are to work with. JavaScript has some seriously murky data structure patterns. I learned JS initially by getting deep into the prototype system. Then ECMA Script 2015 brought classes into play. Both of which require a lot of reading to get back up to speed on. While I'm not averse to extra reading, I want something that feels better..

Enter factory functions.
```js
const thing = (input) => {
    // construct and set private variables.
    const name = parse(input);

    return {
        // object with interface
        getName: () => name,
    };
};
```
This feels like the JavaScript version of simple C structs. I look forward to using them more!
