#pragma once

#include <cstdlib>
#include <iostream>

#include <smp/components/samplers/cliff.h>

#include <smp/common/region.hpp>
#include <smp/components/samplers/base.hpp>

template <class typeparams, int NUM_DIMENSIONS>
int smp::sampler_cliff<typeparams, NUM_DIMENSIONS>::sm_update_insert_vertex(
    vertex_t *vertex_in) {

  return 1;
}

template <class typeparams, int NUM_DIMENSIONS>
int smp::sampler_cliff<typeparams, NUM_DIMENSIONS>::sm_update_insert_edge(
    edge_t *edge_in) {

  return 1;
}

template <class typeparams, int NUM_DIMENSIONS>
int smp::sampler_cliff<typeparams, NUM_DIMENSIONS>::sm_update_delete_vertex(
    vertex_t *vertex_in) {

  return 1;
}

template <class typeparams, int NUM_DIMENSIONS>
int smp::sampler_cliff<typeparams, NUM_DIMENSIONS>::sm_update_delete_edge(
    edge_t *edge_in) {

  return 1;
}

template <class typeparams, int NUM_DIMENSIONS>
smp::sampler_cliff<typeparams, NUM_DIMENSIONS>::sampler_cliff(
    const cliffmap_ros::CLiFFMapConstPtr &map) {
  this->set_support(map);
  rejections = 0;
}

template <class typeparams, int NUM_DIMENSIONS>
smp::sampler_cliff<typeparams, NUM_DIMENSIONS>::sampler_cliff() {
  rejections = 0;
}

template <class typeparams, int NUM_DIMENSIONS>
smp::sampler_cliff<typeparams, NUM_DIMENSIONS>::~sampler_cliff() {}

template <class typeparams, int NUM_DIMENSIONS>
unsigned int
smp::sampler_cliff<typeparams, NUM_DIMENSIONS>::get_total_rejections() {
  return rejections;
}

template <class typeparams, int NUM_DIMENSIONS>
int smp::sampler_cliff<typeparams, NUM_DIMENSIONS>::sample(
    state_t **state_sample_out) {

  if (NUM_DIMENSIONS <= 0)
    return 0;

  state_t *state_new = new state_t;

  bool accept = false;
  double randnum = double(rand()) / double(RAND_MAX);

  for (int i = 0;; i++) {

    // Generate an independent random variable for each axis.
    for (int i = 0; i < NUM_DIMENSIONS; i++)
      (*state_new)[i] = support.size[i] * rand() / (RAND_MAX + 1.0) -
                        support.size[i] / 2.0 + support.center[i];

    double x = state_new->state_vars[0];
    double y = state_new->state_vars[1];
    double prob = (*cliffmap)(x, y).p * (1-(*cliffmap)(x, y).q);

    if (prob > randnum) {
      rejections += i;
      //printf("\n%lf > %lf", prob, randnum);
      break;
    }
    else {
      //printf("\n%lf < %lf", prob, randnum);
    }
  }

  *state_sample_out = state_new;

  return 1;
}

template <class typeparams, int NUM_DIMENSIONS>
int smp::sampler_cliff<typeparams, NUM_DIMENSIONS>::set_support(
    const cliffmap_ros::CLiFFMapConstPtr &map) {
  cliffmap = map;
  support.center[0] = 0.0;
  support.size[0] = cliffmap->getXMax() - cliffmap->getXMin();
  support.center[1] = 0.0;
  support.size[1] = cliffmap->getYMax() - cliffmap->getYMin();
  support.center[2] = 0.0;
  support.size[2] = 2 * M_PI;
}