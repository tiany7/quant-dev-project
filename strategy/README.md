# Introduction
This is a simple strategy framework that can be used to develop trading strategies. 


# Overview
All strategies are derived from the `BaseStrategy` class, which is defined in `base_strategy.h`. The `BaseStrategy` 
class has the following methods:

```text
ctor
dtor
apply_strategy
```
More functions will be added in the future.

Each strategy class has two main members, input and output. Input and output are unbounded mpsc channels that can receive the input from the upstream and spit out the output to the downstream. To understand the implementation defails of this channel, please refer to `utils.h` for more details.


# Files

```text
*_strategy.h/.cpp: strategy implementation
CSVReader.h/.cpp: csv reader
utils.h/.cpp: utility functions and data structures
const_def.h: constant definitions
```
