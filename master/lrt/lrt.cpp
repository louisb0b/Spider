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

#include <lrt.h>
#include <LrtCommunicator.h>
#include <Message.h>
#include <platform.h>
#include <tools/DynStack.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "specialActors/specialActors.h"

static lrtFct specialActors[6] = {
		&saBroadcast,
		&saFork,
		&saJoin,
		&saRoundbuffer,
		&saInit,
		&saEnd
};

LRT::LRT(int ix){
	/* TODO add some heapMemory */
	fcts_ = 0;
	nFct_ = 0;
	ix_ = ix;
	run_ = false;
	stack_ = new DynStack("LRTStack");
}
LRT::~LRT(){
	/* Nothing to Unalloc */
	delete stack_;
}

void LRT::setFctTbl(const lrtFct fct[], int nFct){
	fcts_ = fct;
	nFct_ = nFct;
}

void LRT::sendTrace(int srdagIx, Time start, Time end){
	TraceMsg* msgTrace = (TraceMsg*) Platform::getLrtCommunicator()->trace_start_send(sizeof(TraceMsg));

	msgTrace->msgIx = TRACE_JOB;
	msgTrace->srdagIx = srdagIx;
	msgTrace->spiderTask = (unsigned long)-1;
	msgTrace->start = start;
	msgTrace->end = end;
	msgTrace->lrtIx = ix_;

	Platform::getLrtCommunicator()->trace_end_send(sizeof(TraceMsgType));
}

int LRT::runOneJob(){
	void* msg;
	if(Platform::getLrtCommunicator()->ctrl_start_recv(&msg)){
		switch(((UndefinedMsg*) msg)->msgIx){
		case MSG_START_JOB:{
			StartJobMsg* jobMsg = (StartJobMsg*) msg;
			Fifo *inFifos = (Fifo*) ((char*)jobMsg + 1*sizeof(StartJobMsg));
			Fifo *outFifos = (Fifo*) ((char*)inFifos + jobMsg->nbInEdge*sizeof(Fifo));
			Param *inParams = (Param*) ((char*)outFifos + jobMsg->nbOutEdge*sizeof(Fifo));

			void** inFifosAlloc = CREATE_MUL(stack_, jobMsg->nbInEdge, void*);
			void** outFifosAlloc = CREATE_MUL(stack_, jobMsg->nbOutEdge, void*);
			Param* outParams = CREATE_MUL(stack_, jobMsg->nbOutParam, Param);

			for(int i=0; i<(int)jobMsg->nbInEdge; i++){
				inFifosAlloc[i] = (void*) Platform::getLrtCommunicator()->data_recv(&inFifos[i]);
			}

			for(int i=0; i<(int)jobMsg->nbOutEdge; i++){
				outFifosAlloc[i] = (void*) Platform::getLrtCommunicator()->data_start_send(&outFifos[i]);
			}

			Time start = Platform::get()->getTime();

			if(jobMsg->specialActor && jobMsg->fctIx < 6)
				specialActors[jobMsg->fctIx](inFifosAlloc, outFifosAlloc, inParams, outParams);
			else if((int)jobMsg->fctIx < nFct_)
				fcts_[jobMsg->fctIx](inFifosAlloc, outFifosAlloc, inParams, outParams);
			else
				throw "Cannot find actor function\n";

			Time end = Platform::get()->getTime();

			sendTrace(jobMsg->srdagIx, start, end);

			for(int i=0; i<(int)jobMsg->nbOutEdge; i++){
				Platform::getLrtCommunicator()->data_end_send(&outFifos[i]);
			}

			if(jobMsg->nbOutParam != 0){
				int size = sizeof(ParamValueMsg)+jobMsg->nbOutParam*sizeof(Param);
				ParamValueMsg* msgParam = (ParamValueMsg*) Platform::getLrtCommunicator()->ctrl_start_send(size);
				Param* params = (Param*)(msgParam+1);

				msgParam->msgIx = MSG_PARAM_VALUE;
				msgParam->srdagIx = jobMsg->srdagIx;
				memcpy(params, outParams, jobMsg->nbOutParam*sizeof(Param));

				Platform::getLrtCommunicator()->ctrl_end_send(size);
			}

			stack_->free(inFifosAlloc);
			stack_->free(outFifosAlloc);
			stack_->free(outParams);
			stack_->freeAll();
			break;
		}
		case MSG_CLEAR_TIME:{
			ClearTimeMsg* timeMsg = (ClearTimeMsg*) msg;
			Platform::get()->rstTime(timeMsg);
			break;}
		case MSG_STOP_LRT:
			run_ = false;
			break;
		case MSG_PARAM_VALUE:
		default:
			throw "Unexpected message received\n";
		}
		Platform::getLrtCommunicator()->ctrl_end_recv();
		return 1;
	}
	return 0;
}

void LRT::runUntilNoMoreJobs(){
	while(runOneJob());
}

void LRT::runInfinitly(){
	run_ = true;
	do{
		runOneJob();
	}while(run_);
}
