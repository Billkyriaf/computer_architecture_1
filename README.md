<div id="top"></div>

<br />
<div align="center">
  <h1 align="center">Computer Architecture Assignment 1</h1>
  <h3 align="center">Aristotle University of Thessaloniki</h3>
  <h4 align="center">School of Electrical & Computer Engineering</h4>
  <p align="center">
    Contributors: Kyriafinis Vasilis, Nikolaos Giannopoulos
    <br />
    Winter Semester 2021 - 2022
    <br />
    <br />
  </p>
</div>
<br />

# 1. Contents
- [1. Contents](#1-contents)
- [2. Simulation Parameters  (Question 1)](#2-simulation-parameters--question-1)
  - [2.1. The main modules of the system](#21-the-main-modules-of-the-system)
- [3. Run Statistics (Question 2)](#3-run-statistics-question-2)
- [4. CPI (Question 3)](#4-cpi-question-3)
- [5. Documentation References (Question 4)](#5-documentation-references-question-4)
  - [5.1. Gem5 CPU models](#51-gem5-cpu-models)
  - [5.2. InOrder CPUs](#52-inorder-cpus)
  - [5.3. InOrder Pipeline Stages](#53-inorder-pipeline-stages)
  - [5.4. InOrder Instruction Schedules](#54-inorder-instruction-schedules)
- [6. Compile and simulate a C program for ARM (Question 4a)](#6-compile-and-simulate-a-c-program-for-arm-question-4a)
- [7. Change parameters and detect differences (Question 4b)](#7-change-parameters-and-detect-differences-question-4b)
  - [7.1. Changing the operation frequency](#71-changing-the-operation-frequency)
  - [7.2. Changing memory technology](#72-changing-memory-technology)
- [8. Sources](#8-sources)
- [9. Σχόλια για την εργασία](#9-σχόλια-για-την-εργασία)

<br/>

# 2. Simulation Parameters  (Question 1)

The file `starter_se.py` contains the script that defines the characteristics of the system to be emulated. The command to run the simulation is:

    ./build/ARM/gem5.opt configs/example/arm/starter_se.py --cpu="minor" "tests/test-progs/hello/bin/arm/linux/hello"

* The `configs/example/arm/starter_se.py` flag is the path to configuration script of the simulation
* The `--cpu="minor"` `"tests/test-progs/hello/bin/arm/linux/hello"` are the command line arguments for the configuration script
  * `--cpu="minor"`: Defines the type of the CPU to be used (default="atomic").
  * `"tests/test-progs/hello/bin/arm/linux/hello"`: The path to the executable binary that will run in the emulator.


In the lines 189 to 208 of the file `starter_se.py` the arguments are parsed. For this simulation only the above arguments were provided, so all the other arguments defaulted to the values specified in the script. 


* `--cpu-freq`: CPU clock frequency. Default = 1GHz
* `--num-cores`: Number of cores of the CPU. Default = 1
* `--mem-type`: Type of system memory. Default = `DDR3_1600_8x8` meaning Transfer rate = 1.6 x 8 x 8 x 1/8 = 12.8GBps
* `--mem-channels`: The number of memory channels. Default = 2
* `--mem-ranks`: Number of ranks per memory channel. Default = None
* `--mem-size`: Memory size. Default = 2GB


## 2.1. The main modules of the system

* **_System Clock_**: The system clock has a default frequency of 1GHz and it is not the same clock with the CPU clock. To change the system clock frequency the value in the below line must be changed.
  
      self.clk_domain = SrcClockDomain(clock="1GHz",voltage_domain=self.voltage_domain)

* **_Memory bus_**: The memory bus connects the CPU with the system memory.
  
      self.membus = SystemXBar()

* **_CPU_**: The type and the frequency of the CPU are determined by the command line arguments. To change the default frequency of the CPU the `--cpu-freq` argument must be passed to the script.
  
      devices.`path/to/binary`(self, args.num_cores, args.cpu_freq, "1.2V", *cpu_types[args.cpu])


Depending on the types of the CPU it is determined if the system will have cache memories. If the memory model is not Atomic, which means the data are read from the memory with no delays, 2 cache levels are created. 

* **_L1 Cache_**: L1 cache is private to each core, so that every core can only access his L1 cache. 
  
      self.cpu_cluster.addL1()

* **_L2 Cache_**: L2 Cache is shared between all of the cores.
  
      self.cpu_cluster.addL2(self.cpu_cluster.clk_domain)

<br/>

# 3. Run Statistics (Question 2)

The `stats.txt` file contains information about the simulation from all the SimObjects. At the end of the simulation the statistics are automatically dumped to the file. Some important information can be derived from the bellow values:

* **_sim_seconds_**: Number of seconds simulated (0.000035 s). This is the time the binary took to execute in the simulator.
* **_sim_insts_**: The total number of instructions that where simulated (5027 instructions).
* **_host_inst_rate_**: The instructions per second of the simulator on the host machine (118842 inst/s). Basically this is the performance of the gem5 simulator.

<br />

# 4. CPI (Question 3)

In order to calculate the CPI (Clocks per instruction) the total cache misses for L1 data and instructions and the total cache misses for L2 are needed. Also the miss penalties and hit penalties for both levels of cache and finally the total number of instruction executed are required. The mathematical type is very simple: 

<br/>

<div align="center">
<a href="https://www.codecogs.com/eqnedit.php?latex=\dpi{100}&space;\bg_white&space;\fn_jvn&space;CPI&space;=&space;1&space;&plus;&space;\frac{(\text{IL1.miss\_num&space;&plus;&space;\text{DL1.miss\_num}})\times&space;6&space;&plus;&space;\text{L2.miss\_num}\times&space;50}{\text{Total\_Inst\_num}}" target="_blank"><img src="https://latex.codecogs.com/png.latex?\dpi{100}&space;\bg_white&space;\fn_jvn&space;CPI&space;=&space;1&space;&plus;&space;\frac{(\text{IL1.miss\_num&space;&plus;&space;\text{DL1.miss\_num}})\times&space;6&space;&plus;&space;\text{L2.miss\_num}\times&space;50}{\text{Total\_Inst\_num}}" title="CPI = 1 + \frac{(\text{IL1.miss\_num + \text{DL1.miss\_num}})\times 6 + \text{L2.miss\_num}\times 50}{\text{Total\_Inst\_num}}" /></a>
</div>
<br/>

The first 1 is there because if we miss on the L1 cache we have already paid for the hit penalty. Afterward all the cycles spent dealing with the miss are counted and finally the average per instruction is calculated by dividing with the number of the total instructions.

<br/>

In our case the number of misses on the L1 cache are:

* 327 misses for instruction cache:  
      
      system.cpu_cluster.cpus.icache.demand_misses::.cpu_cluster.cpus.inst  327  # number of demand (read+write) misses

* 177 misses for data cache:
    
      system.cpu_cluster.cpus.dcache.demand_misses::.cpu_cluster.cpus.data  177  # number of demand (read+write) misses

The L2 cache is unified, meaning there is no instruction and data part. So the misses for the L2 cache are calculated by adding the misses from L2 instructions and L2 data misses.

In our case the number of misses on the L2 cache are:

* 327 misses for instructions:
      
      system.cpu_cluster.l2.demand_misses::.cpu_cluster.cpus.inst  327   # number of demand (read+write) misses

* 147 misses for data:
      
      system.cpu_cluster.l2.demand_misses::.cpu_cluster.cpus.inst  327    # number of demand (read+write) misses

The number of instruction misses are the same on the L1 and L2 caches because the different instructions executed fitted inside the L1 instruction cache and the L2 cache. That means the instructions where requested by the CPU and were loaded from the DRAM so for every instruction fetched there was an initial compulsory miss.
<br/>

On the other hand the data misses are more for the L1 cache. This is the case because data where used more than once so after the first compulsory miss and the data fetch from the DRAM to the L2 cache the data were small enough to remain inside L2 cache but not small enough to fit in the L1 cache all at once. So there were cases that data was requested by the CPU the L1 cache got a miss bu the L2 cache got a hit.
<br/>

Finally the total instructions simulated were:

    sim_insts  5027    # Number of instructions simulated

Applying the above data to the equation gives `CPI = 6.32 cycles per instruction`

<br/>

# 5. Documentation References (Question 4)

## 5.1. Gem5 CPU models

* **_SimpleCPU_** - A good place to start learning about how to fetch, decode, execute, and complete instructions in M5.
* **_O3CPU_** - Specific documentation on how all of the pipeline stages work, and how to modify and create new CPU models based on it.
* **_Checker_** - Details how to use it in your CPU model.
* **_InOrderCPU_** - Specific documentation on how all of the pipeline stages work, and how to modify and create new CPU models based on it.

## 5.2. InOrder CPUs
The InOrder CPU model was designed to provide a generic framework to simulate in-order pipelines with an arbitrary ISA and with arbitrary pipeline descriptions. The model was originally conceived by closely mirroring the O3CPU model to provide a simulation framework that would operate at the "Tick" granularity. We then abstract the individual stages in the O3 model to provide generic pipeline stages for the InOrder CPU to leverage in creating a user-defined amount of pipeline stages. Additionally, we abstract each component that a CPU might need to access (ALU, Branch Predictor, etc.) into a "resource" that needs to be requested by each instruction according to the resource-request model we implemented. This will potentially allow for researchers to model custom pipelines without the cost of designing the complete CPU from scratch. 

<br />

## 5.3. InOrder Pipeline Stages
Pipeline stages in the InOrder CPU are implemented as abstract implementations of what a pipeline stage would be in any CPU model. Typically, one would imagine a particularly pipeline stage being responsible for:
  1. Performing specific operations such as "Decode" or "Execute" and either 
  2. Sending that instruction to the next stage if that operation was successful and the next stage's buffer has room for incoming instructions 
  or 
  3. Keeping that instruction in the pipeline's instruction buffer if that operation was unsuccessful or there is no room in the next stage's buffer
  
<br /> 

The "PipelineStage" class maintains the functionality of (2a) and (2b) but abstracts (1) out of the implementation. More specifically, no pipeline stage is explicitly marked "Decode" or "Execute". Instead, the PipelineStage class allows the instruction and it's corresponding instruction schedule to define what tasks they want to do in a particular stage.
        
<br />

## 5.4. InOrder Instruction Schedules

At the heart of the InOrderCPU model is the concept of Instruction Schedules (IS). Instruction schedules create the generic framework that allow for developer's to make a custom pipeline. A pipeline definition can be seen as a collection of instruction schedules that govern what an instruction will do in any given stage and what stage that instruction will go to next. In general, each instruction has a stage-by-stage list of tasks that need to be accomplished before moving on to the next stage. This list we refer to as the instruction's schedule. Each list is composed of "ScheduleEntry"s that define a task for the instruction to do for a given pipeline stage. Instruction scheduling is then divided into a front-end schedule (e.g. Instruction Fetch and Decode) which is uniform for all the instructions, and a back-end schedule, which varies across the different instructions (e.g. a 'addu' instruction and a 'mult' instruction need to access different resources). The combination of a front-end schedule and a back-end schedule make up the instruction schedule. Ideally, changing the pipeline can be as simple as editing how a certain class of instructions operate by editing the instruction schedule functions. 

<br />

# 6. Compile and simulate a C program for ARM (Question 4a)

The simpleExample.c file is a simple program that generates 2 tables with random values of 3x3 floating point values which was compiled with the following command:

  
    arm-linux-gnueabihf-gcc --static tests/test-progs/simplyTableExample/simpleExample.c -o tests/test-progs/simplyTableExample/simpleExample.out 

Then having previously compiled with the command:


    scons build/ARM/gem5.opt -j 2 -force-lto

we run the following command:<br />
    
    ./build/ARM/gem5.opt -d TimeSimpleCPU configs/example/se.py --cmd=tests/test-progs/simplyTableExample/simpleExample.out --cpu-type=TimingSimpleCPU --caches

And we get the [**_TimeSimpleCPU_stats.txt_**](Question_4/TimeSimpleCPU/TimeSimpleCPU_stats.txt) file after the end of the simulation. The process is repeated for the `MinorCPU` model:

    ./build/ARM/gem5.opt -d MinorCPU configs/example/se.py --cmd=tests/test-progs/simplyTableExample/simpleExample.out --cpu-type=MinorCPU --caches

And we get the [**_MinorCPU_stats.txt_**](Question_4/MinorCPU/MinorCPU_stats.txt) file after the end of the simulation:<br />


# 7. Change parameters and detect differences (Question 4b)

We'll try to change the frequency of operation and the memory technology in both cases.
    
## 7.1. Changing the operation frequency

* ### TimingSimpleCPU
  To change the operating frequency for CPU TimingSimpleCPU we execute the following command:
      
      ./build/ARM/gem5.opt -d TimeSimpleCPU_changed configs/example/se.py --cmd=tests/test-progs/simplyTableExample/simpleExample.out --cpu-type=TimingSimpleCPU --cpu-clock=2.5GHz --caches

  Where at the exit you will notice the difference in the "tick"
       
       Exiting @ tick 99151200 because exiting with last active thread context 
   
  Compared with the original results where we hadn't touched the frequency at all

      Exiting @ tick 114677000 because exiting with last active thread context

  The difference between the original frequency and the new frequency, that we defined before, making the subtraction is `114.677.000 - 99.151.200 = 15.525.800`. That's due to the higher frequency but the same size and architecture of memory. TimingSimpleCPU 1GHz is faster with a frequency of 2.5GHz

* ### MinorCPU
  
  To change the operating frequency for CPU MinorCPU we execute the following command: 
    
      ./build/ARM/gem5.opt -d MinorCPU_changed configs/example/se.py --cmd=tests/test-progs/simplyTableExample/simpleExample.out --cpu-type=MinorCPU --cpu-clock=2.5GHz --caches

  The simulation result taken from the exit is:
    
      Exiting @ tick 77432400 because exiting with last active thread context

  Comparing it with the original situation without any change in frequency

      Exiting @ tick 84846000 because exiting with last active thread context
  
  As before, making the difference we get `84.846.000-77.432.400= 7.413.600` and again we end up having a small difference (not like the previous one) with the higher frequency in the MinorCPU processor.

* ### Conclusion
  So we understand that the difference in frequency affects the operation of the processor too much, sometimes it affects it too much like in the first case with the TimingSImpleCPU, sometimes less like the MinorCPU.TimingSimpleCPU uses memory accesses with timing. It delays accesses to cache memory and waits for the memory system response before proceeding. Like the AtomicSimpleCPU, the TimingSimpleCPU is also derived from the BaseSimpleCPU and implements the same set of functions. It defines the port used to connect to the memory and connects the CPU to the cache. It also defines the necessary functions for handling the response from memory to the accesses sent.

<br/>

## 7.2. Changing memory technology

* ### TimingSimpleCPU
  
  To change the RAM architecture for CPU TimingSimpleCPU we execute the following command:

      ./build/ARM/gem5.opt -d TimingSimple__changed_mem_type configs/example/se.py --cmd=tests/test-progs/simplyTableExample/simpleExample.out --cpu-type=TimingSimpleCPU --mem-type=DDR4_2400_16x4 --caches

  Where at the exit as before you will notice the difference in the "tick"

      Exiting @ tick 114281000 because exiting with last active thread context

  Compared to the initial results where we had not touched the memory technology at all

      Exiting @ tick 114677000 because exiting with last active thread context

  The difference between the original memory technology and the new one we defined before removing the ticks is `114,677,000 - 114,281,000 = 396,000`. This is due to the higher GB/s transmission which is `2.4x16x16x4x1/8 = 19.2GBps` but having the same frequency as the original. 

* ### MinorCPU
  
  To change the RAM architecture for CPU MinorCPU we execute the following command:

      ./build/ARM/gem5.opt -d MinorCPU_changed_mem_type configs/example/se.py --cmd=tests/test-progs/simplyTableExample/simpleExample.out --cpu-type=MinorCPU --mem-type=DDR4_2400_16x4 --caches
    
    And the exit is
      
      Exiting @ tick 80777000 because exiting with last active thread context

  Compared to the initial results where we had not touched the memory technology at all

      Exiting @ tick 84846000 because exiting with last active thread context

   As before, making the difference we have `84,846,000 - 80,777,000 = 4,069,000` and again we end up with a small difference (not like the previous one) with the different memory technology in the MinorCPU processor.

* ### Conclusion

  So we understand that the difference in memory technology moderately affects the operation of the processor.The TimingSimpleCPU uses timing memory accesses. But here due to the faster and larger GBps transfer rate of the RAM memory used was better in comparison to the MInorCPU and all the disadvantages created before are negated. But the MinorCPU uses InOrder technology for which it uses a fixed pipeline but with configurable data structures and execution behavior but due to the fixed pipeline the higher the speed of transfers to and from it after the "upper limit" of the pipeline it stops progressing and thus becomes worse than TimingSimpleCPU.

<br/>

# 8. Sources

[A Tutorial on the Gem5 Memory Model](https://nitish2112.github.io/post/gem5-memory-model/)

[Creating SimObjects in the memory system](http://learning.gem5.org/book/part2/memoryobject.html)

[Understanding gem5 statistics and output](http://learning.gem5.org/book/part1/gem5_stats.html)

[Understanding gem5 statistics and output Gem5](https://www.gem5.org/documentation/learning_gem5/part1/gem5_stats/)

[What are system clock and CPU clock](https://cs.stackexchange.com/questions/32149/what-are-system-clock-and-cpu-clock-and-what-are-their-functions)

<br/>

# 9. Σχόλια για την εργασία
