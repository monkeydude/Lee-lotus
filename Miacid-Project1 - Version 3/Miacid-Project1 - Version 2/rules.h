#ifndef RULES_H
#define RULES_H

enum TMove {TM_NONE, TM_START, TM_FORWARD, TM_ATTACK};

struct TRule
{
	TMove thirdLast;
	TMove secondLast;
	TMove last;
	TMove result;
	bool matched;
	int weight;
	int rulenum;

	TRule()
	{
		this->thirdLast = TM_NONE;
		this->secondLast = TM_NONE;
		this->last = TM_NONE;
		this->result = TM_NONE;
		this->matched = 0;
		this->weight = 0;
		this->rulenum = 0;
	}

	void SetRule(int num, TMove conditionA, TMove conditionB, TMove conditionC, TMove resultD)
	{
		this->thirdLast = conditionA;
		this->secondLast = conditionB;
		this->last = conditionC;
		this->result = resultD;
		this->matched = 0;
		this->weight = 0;
		this->rulenum = num;
	}
};

#endif