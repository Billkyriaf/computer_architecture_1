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
  <h1 align ="center" >There are many CPU models which are GEM5 or the CPU categories are as follows </h1>
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
       <h2 align = "left">InOrder Pipeline Stages</h3> 
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
  
  
