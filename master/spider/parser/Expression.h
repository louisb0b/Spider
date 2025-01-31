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

#ifndef PARSER_EXPRESSION_H
#define PARSER_EXPRESSION_H

#include <graphs/PiSDF/PiSDFParam.h>
#include <tools/Stack.h>

struct transfoJob;

class Expression {
public:
	Expression(const char* expr, const PiSDFParam* const * params, int nParam, Stack* stack);
	virtual ~Expression();

	int evaluate(const PiSDFParam* const * paramList, transfoJob* job) const;
	int evaluate(const int* vertexParamValues, int nParam) const;
	void toString(
			const PiSDFParam * const * params, int nParam,
			char* out, int outSizeMax);

private:
	typedef enum{
		OPERATOR,
		VALUE,
		PARAMETER,
		LEFT_PAR,
		RIGHT_PAR
	} Type;

	typedef enum{
		ADD,
		SUB,
		MUL,
		DIV,
		POW
	} OpType;

	typedef struct{
		Type 	type;
		OpType 	opType;
		int 	value;
		int 	paramIx;
	} Token;

	Token* stack_;
	int nElt_;
	Stack* stackAlloc_;

	bool getNextToken(
			Token* token,
			const char** expr,
			const PiSDFParam* const * params, int nParam);

	int evaluateNTokens(const char* expr);
};

#endif/*PARSER_EXPRESSION_H*/
