<div id="top"></div>

<br />
<div align="center">
  <h1 align="center">Computer Arcitecture Assignment 1</h1>
  <h3 align="center">Aristotle University of Thessaloniki</h3>
  <h4 align="center">School of Electrical & Computer Engineering</h4>
  <p align="center">
    Contributors: Kyriafinis Vasilis, Nikolaos Giannopoulos
    <br />
    Winter Semester 2021 - 2022
    <br />
    <br />
    <br />
    <br />
  </p>
</div>
<br />

<div align = "center">
  <h1 align ="center" >There are many CPU models included in GEM5.The CPU categories are as follows </h1>
  <h3 align ="left"> 
    <ul>
      <li>SimpleCPU - A good place to start learning about how to fetch, decode, execute, and complete instructions in M5.</li>
      <li>O3CPU - Specific documentation on how all of the pipeline stages work, and how to modify and create new CPU models based on it. </li>
      <li>Checker - Details how to use it in your CPU model. </li>
      <li>InOrderCPU - Specific documentation on how all of the pipeline stages work, and how to modify and create new CPU models based on it.</li>
    </ul>
  </div>
  
  <br />
    <div align = "left">
       <h2 align = "left" >InOrder CPUs</h3> 
      <p align = "left">
&nbsp;&nbsp;&nbsp;&nbsp;The InOrder CPU model was designed to provide a generic framework to simulate in-order pipelines with an arbitrary ISA and with arbitrary pipeline descriptions. The model was originally conceived by closely mirroring the O3CPU model to provide a simulation framework that would operate at the "Tick" granularity. We then abstract the individual stages in the O3 model to provide generic pipeline stages for the InOrder CPU to leverage in creating a user-defined amount of pipeline stages. Additionally, we abstract each component that a CPU might need to access (ALU, Branch Predictor, etc.) into a "resource" that needs to be requested by each instruction according to the resource-request model we implemented. This will potentially allow for researchers to model custom pipelines without the cost of designing the complete CPU from scratch. 
      </p>
  </div>
  <br />
  
  <div align = "left">
       <h2 align = "left">InOrder Pipeline Stages</h2> 
      <p align = "left">
          &nbsp;&nbsp;&nbsp;&nbsp;Pipeline stages in the InOrder CPU are implemented as abstract implementations of what a pipeline stage would be in any CPU model. Typically, one would imagine a particularly pipeline stage being responsible for:<br />
  (1) Performing specific operations such as "Decode" or "Execute" and either <br />
  (2a) Sending that instruction to the next stage if that operation was successful and the next stage's buffer has room for incoming instructions <br />
  or <br />
  (2b) Keeping that instruction in the pipeline's instruction buffer if that operation was unsuccesful or there is no room in the next stage's buffer<br /> 

  &nbsp;&nbsp;&nbsp;&nbsp;The "PipelineStage" class maintains the functionality of (2a) and (2b) but abstracts (1) out of the implementation. More specifically, no pipeline stage is explicitly marked "Decode" or "Execute". Instead, the PipelineStage class allows the instruction and it's corresponding instruction schedule to define what tasks they want to do in a particular stage.
      </p>
  </div>
  
   <br />
  
  <div align = "left">
       <h2 align = "left">InOrder Instruction Schedules</h3> 
      <p align = "left">
  &nbsp;&nbsp;&nbsp;&nbsp;At the heart of the InOrderCPU model is the concept of Instruction Schedules (IS). Instruction schedules create the generic framework that allow for developer's to make a custom pipeline. A pipeline definition can be seen as a collection of instruction schedules that govern what an instruction will do in any given stage and what stage that instruction will go to next. 
  In general, each instruction has a stage-by-stage list of tasks that need to be accomplished before moving on to the next stage. This list we refer to as the instruction's schedule. Each list is composed of "ScheduleEntry"s that define a task for the instruction to do for a given pipeline stage. 
  Instruction scheduling is then divided into a front-end schedule (e.g. Instruction Fetch and Decode) which is uniform for all the instructions, and a back-end schedule, which varies across the different instructions (e.g. a 'addu' instruction and a 'mult' instruction need to access different resources). 
  The combination of a front-end schedule and a back-end schedule make up the instruction schedule. Ideally, changing the pipeline can be as simple as editing how a certain class of instructions operate by editing the instruction schedule functions. 
  </p>
</div>
  
 <br />
<br />
  
<div align = "left">
  <h2 align = "center"> Question : Using different CPU models and keeping all other parameters the same. Use the TimingSimpleCPU and MinorCPU.</h2>
  <br />
  <div align = "left">
   &nbsp;&nbsp;&nbsp;&nbsp;The simpleExample.c file is a simple program that generates 2 tables with random values of 3x3 floating point values which was compiled with the following command:<br />
  
  <h4>arm-linux-gnueabihf-gcc --static tests/test-progs/simplyTableExample/simpleExample.c -o tests/test-progs/simplyTableExample/simpleExample.out </h4><br>
  Then having previously compiled with the command:<br />
    <h4>scons build/ARM/gem5.opt -j 2 -force-lto </h4><br />
  then we run the following command:<br />
  <h4>./build/ARM/gem5.opt -d TimeSimpleCPU configs/example/se.py --cmd=tests/test-progs/simplyTableExample/simpleExample.out --cpu-type=TimingSimpleCPU --caches </h4> <br />
  And we get the following file after the end of the simulation:<br />
    <h4>TimeSimpleCPU/TimeSimpleCPU_stats.txt </h4>
  and as before we're running the: <br />
  <h4>./build/ARM/gem5.opt -d MinorCPU configs/example/se.py --cmd=tests/test-progs/simplyTableExample/simpleExample.out --cpu-type=MinorCPU --caches</h4> <br />
  And we get the following file after the end of the simulation:<br />
    <h4>MinorCPU/MinorCPU_stats.txt</h4> <br />
 </div>
  
</div>
  
  <div align = "left">
  <h2 align = "center"> Question : Change a parameter of the editor and observe the results for both different CPU models.</h2>
  <br />
  <div align = "left">
   &nbsp;&nbsp;&nbsp;&nbsp;We'll try to change the frequency of operation in both cases and the memory technology. <br />
    first case of changing the operating frequency we execute the following command for CPU TimingSimpleCPU<br />
    <h4>./build/ARM/gem5.opt -d TimeSimpleCPU_changed configs/example/se.py --cmd=tests/test-progs/simplyTableExample/simpleExample.out --cpu-type=TimingSimpleCPU --cpu-clock=2.5GHz --caches </h4><br />
    &nbsp;&nbsp;&nbsp;&nbsp;where at the exit you will notice the difference in the "tick"<br />
   <h4> Exiting @ tick 99151200 because exiting with last active thread context </h4><br />
   &nbsp;&nbsp;&nbsp;&nbsp; Compared to the original results where we hadn't touched the frequency at all <br />
    <h4> Exiting @ tick 114677000 because exiting with last active thread context </h4> <br />
    &nbsp;&nbsp;&nbsp;&nbsp;The difference between the original frequency and the new frequency that we defined before making the subtraction is 114.677.000 - 99.151.200=15.525.800
That's due to the higher frequency but the same size and architectural memory.Where TimingSimpleCPU 1GHz is better than the same but with a frequency of 2.5GHz<br />
    <br />
  <div align = "left">Now as before we will do the analysis for the different frequency for the architectural CPU MinorCPU.
    <h4> ./build/ARM/gem5.opt -d MinorCPU_changed configs/example/se.py --cmd=tests/test-progs/simplyTableExample/simpleExample.out --cpu-type=MinorCPU --cpu-clock=2.5GHz --caches</h4><br /> 
    &nbsp;&nbsp;&nbsp;&nbsp;and is taken to the exit
    <h4>Exiting @ tick 77432400 because exiting with last active thread context</h4> <br />
   &nbsp;&nbsp;&nbsp;&nbsp;Comparing it with the original situation without any change in frequency <br />
    <h4>Exiting @ tick 84846000 because exiting with last active thread context</h4> <br />
  &nbsp;&nbsp;&nbsp;&nbsp; As before, making the difference we get 84.846.000-77.432.400= 7.413.600 and again we end up having a small difference (not like the previous one) with the higher frequency in the MinorCPU processor. <br />
    &nbsp;&nbsp;&nbsp;&nbsp;So we understand that the difference in frequency affects the operation of the processor too much, sometimes it affects it too much like in the first case with the TimingSImpleCPU, sometimes less like the MinorCPU.TimingSimpleCPU uses memory accesses with timing. It delays accesses to cache memory and waits for the memory system response before proceeding. Like the AtomicSimpleCPU, the TimingSimpleCPU is also derived from the BaseSimpleCPU and implements the same set of functions. It defines the port used to connect to the memory and connects the CPU to the cache. It also defines the necessary functions for handling the response from memory to the accesses sent.
 </div>
    
<br />

<div align = "left">&nbsp;&nbsp;&nbsp;&nbsp;Now we will see the shift in memory technology for each case first case of changing the RAM architecture we execute the following command for CPU TimingSimpleCPU<br />
    <h4>./build/ARM/gem5.opt -d TimingSimple__changed_mem_type configs/example/se.py --cmd=tests/test-progs/simplyTableExample/simpleExample.out --cpu-type=TimingSimpleCPU --mem-type=DDR4_2400_16x4 --caches</h4><br />
    &nbsp;&nbsp;&nbsp;&nbsp;where at the exit as before you will notice the difference in the "tick"<br />
   <h4> Exiting @ tick 114281000 because exiting with last active thread context </h4><br />
   &nbsp;&nbsp;&nbsp;&nbsp; Compared to the initial results where we had not touched the memory technology at all<br />
    <h4> Exiting @ tick 114677000 because exiting with last active thread context </h4> <br />
    &nbsp;&nbsp;&nbsp;&nbsp;The difference between the original memory technology and the new one we defined before removing the ticks is 114.677.000 - 114281000=396.000 
This is due to the higher GB/s transmission which is 2.4 * 16*16*4*1/8 = 19.2GBps but having the same frequency as the original. <br />
    <br />
  <div align = "left">Now, as before, we will do the analysis for the different memory primitives in the MinorCPU processor.<br />
    <h4> ./build/ARM/gem5.opt -d MinorCPU_changed_mem_type configs/example/se.py --cmd=tests/test-progs/simplyTableExample/simpleExample.out --cpu-type=MinorCPU --mem-type=DDR4_2400_16x4 --caches</h4><br /> 
    &nbsp;&nbsp;&nbsp;&nbsp;and is taken to the exit
    <h4>Exiting @ tick 80777000 because exiting with last active thread context</h4> <br />
   &nbsp;&nbsp;&nbsp;&nbsp;Compared to the initial results where we had not touched the memory technology at all <br />
    <h4>Exiting @ tick 84846000 because exiting with last active thread context</h4> <br />
   &nbsp;&nbsp;&nbsp;&nbsp; As before, making the difference we have 84.846.000-80.777.000= 4.069.000 and again we end up with a small difference (not like the previous one) with the different memory technology in the MinorCPU processor.<br />
    &nbsp;&nbsp;&nbsp;&nbsp;So we understand that the difference in memory technology moderately affects the operation of the processor.The TimingSimpleCPU uses timing memory accesses. But here due to the faster and larger GBps transfer rate of the RAM memory used was better in comparison to the MInorCPU and all the disadvantages created before are negated. But the MinorCPU uses InOrder technology for which it uses a fixed pipeline but with configurable data structures and execution behavior but due to the fixed pipeline the higher the speed of transfers to and from it after the "upper limit" of the pipeline it stops progressing and thus becomes worse than TimingSimpleCPU.
    </div>
    
    
</div>
  
