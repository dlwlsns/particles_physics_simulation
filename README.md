# Physic Engine for Spherical Particles

This repository contains my thesis project for my Bachelor's degree in Computer Science Engineering.

## Abstract
In this thesis physically based particle simulators were analysed and based on the findings a solution was implemented that can handle huge amounts of sphere particle interactions in real-time harnessing the capabilities of modern GPUs. The program efficiently models dynamic interactions and behaviours among particles, focusing on realistic particle motions, forces, and collisions. By skilfully utilizing GPU resources, it becomes a potent tool for understanding and visualizing intricate particle behaviours within the accelerated realm of parallel computing. This program offers a comprehensive approach to exploring particle dynamics in diverse scenarios.

![[50k_spheres.mp4]]

The project is built in C++ with OpenGL (FreeGLUT), taking advantage of compute shaders, instancing, and grid partitioning to enhance the parallelism capabilities of a modern GPU. 

Further information can be found inside my [Thesis documentation](./extra/DOC_RAVANI.pdf)