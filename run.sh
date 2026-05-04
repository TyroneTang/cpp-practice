#!/bin/bash

docker build --network=host \
             --target base \
             --tag localhost/cpp-base:dev \
             --file Dockerfile .