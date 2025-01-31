/****************************************************************************
 * Copyright or © or Copr. IETR/INSA (2013): Julien Heulot, Yaset Oliva,    *
 * Maxime Pelcat, Jean-François Nezan, Jean-Christophe Prevotet             *
 *                                                                          *
 * [jheulot,yoliva,mpelcat,jnezan,jprevote]@insa-rennes.fr                  *
 *                                                                          *
 * This software is a computer program whose purpose is to execute          *
 * parallel applications.                                                   *
 *                                                                          *
 * This software is governed by the CeCILL-C license under French law and   *
 * abiding by the rules of distribution of free software.  You can  use,    *
 * modify and/ or redistribute the software under the terms of the CeCILL-C *
 * license as circulated by CEA, CNRS and INRIA at the following URL        *
 * "http://www.cecill.info".                                                *
 *                                                                          *
 * As a counterpart to the access to the source code and  rights to copy,   *
 * modify and redistribute granted by the license, users are provided only  *
 * with a limited warranty  and the software's author,  the holder of the   *
 * economic rights,  and the successive licensors  have only  limited       *
 * liability.                                                               *
 *                                                                          *
 * In this respect, the user's attention is drawn to the risks associated   *
 * with loading,  using,  modifying and/or developing or reproducing the    *
 * software by the user in light of its specific status of free software,   *
 * that may mean  that it is complicated to manipulate,  and  that  also    *
 * therefore means  that it is reserved for developers  and  experienced    *
 * professionals having in-depth computer knowledge. Users are therefore    *
 * encouraged to load and test the software's suitability as regards their  *
 * requirements in conditions enabling the security of their systems and/or *
 * data to be ensured and,  more generally, to use and operate it in the    *
 * same conditions as regards security.                                     *
 *                                                                          *
 * The fact that you are presently reading this means that you have had     *
 * knowledge of the CeCILL-C license and that you accept its terms.         *
 ****************************************************************************/

#ifndef SPIDER_H
#define SPIDER_H

#include <graphs/PiSDF/PiSDFCommon.h>
#include <graphs/SRDAG/SRDAGCommon.h>
#include <graphs/PiSDF/PiSDFGraph.h>
#include <graphs/SRDAG/SRDAGGraph.h>
#include <graphs/PiSDF/PiSDFVertex.h>
#include <graphs/Bipartite/BipartiteGraph.h>

#include <tools/StaticStack.h>
#include <tools/DynStack.h>

#include <graphs/Archi/Archi.h>
#include <graphs/Archi/SharedMemArchi.h>

#include <scheduling/MemAlloc.h>
#include <scheduling/MemAlloc/DummyMemAlloc.h>
#include <scheduling/MemAlloc/SpecialActorMemAlloc.h>
#include <scheduling/Scheduler.h>
#include <scheduling/Scheduler/ListScheduler.h>

#include <graphTransfo/GraphTransfo.h>

#include <SpiderCommunicator.h>
#include <lrt.h>

#define MAX_STATS_VERTICES 1000
#define MAX_STATS_PE_TYPES 3

typedef struct{
	StackType type;
	const char* name;

	void* start;
	int size;
} StackConfig;

typedef struct{
	MemAllocType memAllocType;
	void* memAllocStart;
	int memAllocSize;

	SchedulerType schedulerType;

	StackConfig srdagStack;
	StackConfig transfoStack;
} SpiderConfig;

typedef struct{
	Time globalEndTime;
	Time endTime;

	Time taskOrderingTime;
	Time mappingTime;
	Time graphTransfoTime;

	Time actorTimes[MAX_STATS_VERTICES][MAX_STATS_PE_TYPES];
	Time actorFisrt[MAX_STATS_VERTICES];
	Time actorLast[MAX_STATS_VERTICES];
	Time actorIterations[MAX_STATS_VERTICES][MAX_STATS_PE_TYPES];
	PiSDFVertex* actors[MAX_STATS_VERTICES];
	int nbActor;

	Time forkTime;
	Time joinTime;
	Time rbTime;
	Time brTime;

	int SRDAGVertices;
	int SRDAGEdges;

	int memoryUsed;

	Time latencies;
}ExecutionStat;

void spider_init(SpiderConfig cfg);
void spider_launch(Archi* archi, PiSDFGraph* pisdf);
void spider_free();

void spider_setMemAllocType(MemAllocType type, int start, int size);
void spider_setSchedulerType(SchedulerType type);
void spider_setSrdagStack(StackConfig cfg);
void spider_setTransfoStack(StackConfig cfg);

void spider_startMonitoring();
void spider_endMonitoring(TraceSpiderType type);

void spider_printGantt(Archi* archi, SRDAGGraph* srdag, const char* ganttPath, const char* latexPath, ExecutionStat* stat);

SRDAGGraph* spider_getLastSRDAG();

#endif//SPIDER_H
