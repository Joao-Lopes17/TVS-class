# Operating Systems – Practical Assignments

This repository contains four practical assignments (`cw1` to `cw4`) developed for the Operating Systems course. Each assignment explores different aspects of interacting with the operating system, from implementing a shell in C to orchestrating services with Docker.

## Index

- [Assignment 1 (cw1) – `bach` Shell](#assignment-1-cw1--bach-shell)
- [Assignment 2 (cw2) – Assembly, ARM64, and Virtual Address Space](#assignment-2-cw2--assembly-arm64-and-virtual-address-space)
- [Assignment 3 (cw3) – App with Elasticsearch and Nginx](#assignment-3-cw3--app-with-elasticsearch-and-nginx)
- [Assignment 4 (cw4) – Orchestration with Docker](#assignment-4-cw4--orchestration-with-docker)

---

## Assignment 1 (cw1) – `bach` Shell

### Description

A simplified shell implemented in C called `bach`. It supports:

- Execution of commands with arguments
- Input/output redirection
- Pipelining commands using `|`
- Directory changes using `cd`
- Exiting the shell with the `exit` command

## Assignment 2 (cw2) – Assembly, ARM64, and Virtual Address Space
Part 1 – Assembly Analysis
Analysis of x86-64 assembly code snippets based on the AMD64 architecture manual. Identification and discussion of instruction behavior.

Part 2 – ARM64 Conversion
Conversion of x86-64 code into equivalent ARM64 code, preserving its functionality.

Part 3 – Virtual Address Space
Study of the Linux virtual address space by running a C program that performs memory allocations. Includes:

Exploration of how a C program maps into virtual memory. Includes:

- Use of pmap to inspect memory segments
- Examination of code, heap, stack, and mapped regions
- Observations about how variables and dynamically allocated memory are arranged

## Assignment 3 (cw3) – App with Elasticsearch and Nginx

### Description
Deployment of a Node.js application that:

- Accepts HTTP requests via a Unix socket
- Forwards queries to Elasticsearch
- Is reverse-proxied by Nginx

Includes:

- Custom Bash scripts to start/stop the environment
- A systemd socket/service pair
- Communication through /run/app.sock

Requirements:

- Node.js
- Elasticsearch installed locally
- Nginx
- systemd available and enabled

## Assignment 4 (cw4) – Orchestration with Docker

### Description
Containerized version of the same application setup as cw3, using Docker and Docker Compose. It includes:

- A Node.js app container
- An Elasticsearch service with persistent storage
- An Nginx reverse proxy container
- Multiple replicas of the application behind Nginx


Requirements:

- Docker
- Docker Compose
