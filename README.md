# Advanced Computer Architecture - Coursework

## Rahat Muneeb

## Compilation and Execution

## Use the command `make` from root directory

## To run use the command `./build/out`

## Configuration file
```
The simulator can be configured with config.json file
{
  "debug": { // debug mode
    "print": false, // enable prints
    "enabled": false, // enable step mode
    "till": 0 // execute till cycle and then enable step mode
  },
  "program": "matmul.asm", // program to execute
  "units": { // configure unit and superscalar width
    "commit": 8,
    "decode": 8,
    "execute":8,
    "memory": 8
  },
  "speculate":
  {
    "take_branch": true // speculative execution (true = always take), (false =  never take)
  },
  "output": {
    "all" : false, // output all bytes
    "num_bytes": 21000, // output number of bytes
    "filename": "sim.out" // Output filename for main memory
  },
  "capacity": {
    "rsv": 64, // Number of Entries in RSV
    "rob": 64, // Number of Entries in ROB
    "lsq": 64, // Number of entries in LSQ
    "instrQ": 8 // Size of Instruction Queue
  },
  "predictor": 1, // Branch predictor (0 = Speculative Execution), (1 = 1Bit Counter), (2 = 2Bit Counter)
  "stop_time": 0 // stop time between instructions
}
```
