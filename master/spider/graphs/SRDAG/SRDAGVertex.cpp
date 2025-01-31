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


#include <graphs/SRDAG/SRDAGVertex.h>
#include <stdio.h>
#include <string.h>

/** Static Var def */
int SRDAGVertex::globalId = 0;

/** Constructor */
SRDAGVertex::SRDAGVertex(
		SRDAGType type,	SRDAGGraph* graph,
		PiSDFVertex* reference,
		int nInEdge, int nOutEdge,
		int nInParam, int nOutParam,
		Stack* stack){
	id_ = globalId++;

	stack_ = stack;
	type_ = type;
	state_ = SRDAG_NEXEC;
	graph_ = graph;
	reference_ = reference;

	nInEdge_ = nInEdge;
	inEdges_ = CREATE_MUL(stack, nInEdge_, SRDAGEdge*);
	memset(inEdges_, 0, nInEdge_*sizeof(SRDAGEdge*));

	nOutEdge_ = nOutEdge;
	outEdges_ = CREATE_MUL(stack, nOutEdge_, SRDAGEdge*);
	memset(outEdges_, 0, nOutEdge_*sizeof(SRDAGEdge*));

	nInParam_ = nInParam;
	inParams_ = CREATE_MUL(stack, nInParam_, int);
	memset(inParams_, 0, nInParam*sizeof(int));

	nOutParam_ = nOutParam;
	outParams_ = CREATE_MUL(stack, nOutParam_, int*);
	memset(outParams_, 0, nOutParam*sizeof(int**));

	start_ = end_ = -1;
	schedLvl_ = -1;
	slave_ = -1;
}

SRDAGVertex::~SRDAGVertex(){
	stack_->free(inEdges_);
	stack_->free(outEdges_);
	stack_->free(inParams_);
	stack_->free(outParams_);
}

void SRDAGVertex::toString(char* name, int sizeMax) const{
	switch(type_){
	case SRDAG_NORMAL:
		snprintf(name, sizeMax, "%s", reference_->getName());
		break;
	case SRDAG_FORK:
		snprintf(name, sizeMax, "Fork");
		break;
	case SRDAG_JOIN:
		snprintf(name, sizeMax, "Join");
		break;
	case SRDAG_ROUNDBUFFER:
		snprintf(name, sizeMax, "RB");
		break;
	case SRDAG_BROADCAST:
		snprintf(name, sizeMax, "BR");
		break;
	case SRDAG_INIT:
		snprintf(name, sizeMax, "Init");
		break;
	case SRDAG_END:
		snprintf(name, sizeMax, "End");
		break;
	}
}

void SRDAGVertex::updateState(){
	if(state_ == SRDAG_NEXEC){
		/* Check Input Edges */
		for (int i = 0; i < getNConnectedInEdge(); i++){
			SRDAGVertex* predecessor = getInEdge(i)->getSrc();

			if(!predecessor || predecessor->isHierarchical()){
				state_ = SRDAG_NEXEC;
				return;
			}

			if(predecessor->state_ == SRDAG_NEXEC){
				predecessor->updateState();
				if(predecessor->state_ == SRDAG_NEXEC){
					state_ = SRDAG_NEXEC;
					return;
				}
			}
		}
		state_ = SRDAG_EXEC;
	}
}
