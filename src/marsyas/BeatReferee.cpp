/*
** Copyright (C) 1998-2006 George Tzanetakis <gtzan@cs.uvic.ca>
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** \
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "BeatReferee.h"

using namespace std;
using namespace Marsyas;

#define NONE 0.0
#define BEAT 1.0
#define EVAL 2.0

#define INNER 3.0
#define OUTTER 4.0

BeatReferee::BeatReferee(string name):MarSystem("BeatReferee", name)
{
	addControls();

  	bestScore_ = 0.0;
	bestAgentIndex_ = 0; //0 by default
	t_ = 0;
	outputCount_ = 0;
	lastBeatTime_ = -70;
	inductionFinnished_ = false;
}

BeatReferee::BeatReferee(const BeatReferee& a) : MarSystem(a)
{
  // For any MarControlPtr in a MarSystem 
  // it is necessary to perform this getctrl 
  // in the copy constructor in order for cloning to work 
	ctrl_mutedAgents_ = getctrl("mrs_realvec/mutedAgents");
	ctrl_inductionEnabler_ = getctrl("mrs_realvec/inductionEnabler");
	ctrl_firstHypotheses_ = getctrl("mrs_realvec/beatHypotheses");
	ctrl_inductionTime_ = getctrl("mrs_natural/inductionTime");
	ctrl_hopSize_ = getctrl("mrs_natural/hopSize");
	ctrl_srcFs_ = getctrl("mrs_real/srcFs");
	ctrl_maxTempo_ = getctrl("mrs_natural/maxTempo");
	ctrl_minTempo_ = getctrl("mrs_natural/minTempo");
	ctrl_agentControl_ = getctrl("mrs_realvec/agentControl");
	ctrl_beatDetected_ = getctrl("mrs_real/beatDetected");
	ctrl_tickCount_ = getctrl("mrs_natural/tickCount");
	ctrl_obsoleteFactor_ = getctrl("mrs_real/obsoleteFactor");
	ctrl_childrenScoreFactor_ = getctrl("mrs_real/childrenScoreFactor");
	ctrl_bestFactor_ = getctrl("mrs_real/bestFactor");
	ctrl_eqPhase_ = getctrl("mrs_natural/eqPhase");
	ctrl_eqPeriod_ = getctrl("mrs_natural/eqPeriod");
	ctrl_corFactor_ = getctrl("mrs_real/corFactor");
	ctrl_child1Factor_ = getctrl("mrs_real/child1Factor");
	ctrl_child2Factor_ = getctrl("mrs_real/child2Factor");
	ctrl_child3Factor_ = getctrl("mrs_real/child3Factor");
	ctrl_metricalChangeTime_ = getctrl("mrs_natural/metricalChangeTime");
	ctrl_backtrace_ = getctrl("mrs_bool/backtrace");

	t_ = a.t_;
	historyCount_ = a.historyCount_;
	historyBeatTimes_ = a.historyBeatTimes_;
	lastBeatTime_ = a.lastBeatTime_;
	bestScore_ = a.bestScore_;
	bestAgentIndex_ = a.bestAgentIndex_;
	outputCount_ = a.outputCount_;
	statsPeriods_ = a.statsPeriods_;
	statsPhases_ = a.statsPhases_;
	statsAgentsLifeCycle_ = a.statsAgentsLifeCycle_;
	statsMuted_ = a.statsMuted_;
	inductionFinnished_ = a.inductionFinnished_;
	initPeriod_ = a.initPeriod_;
	corFactor_ = a.corFactor_;
	backtrace_ = a.backtrace_;
}

BeatReferee::~BeatReferee()
{
}

MarSystem* 
BeatReferee::clone() const 
{
  return new BeatReferee(*this);
}

void 
BeatReferee::addControls()
{
  //Add specific controls needed by this MarSystem.
	addctrl("mrs_realvec/mutedAgents", realvec(), ctrl_mutedAgents_);
	addctrl("mrs_realvec/inductionEnabler", realvec(), ctrl_inductionEnabler_);
	addctrl("mrs_realvec/beatHypotheses", realvec(), ctrl_firstHypotheses_);
	addctrl("mrs_natural/inductionTime", 5, ctrl_inductionTime_);
	addctrl("mrs_natural/hopSize", -1, ctrl_hopSize_);
	setctrlState("mrs_natural/hopSize", true);
	addctrl("mrs_real/srcFs", -1.0, ctrl_srcFs_);
	setctrlState("mrs_real/srcFs", true);
	addctrl("mrs_natural/maxTempo", 250, ctrl_maxTempo_);
	setctrlState("mrs_natural/maxTempo", true);
	addctrl("mrs_natural/minTempo", 50, ctrl_minTempo_);
	setctrlState("mrs_natural/minTempo", true);
	addctrl("mrs_realvec/agentControl", realvec(), ctrl_agentControl_);
	addctrl("mrs_real/beatDetected", 0.0, ctrl_beatDetected_);
	addctrl("mrs_natural/tickCount", 0, ctrl_tickCount_);
	addctrl("mrs_real/obsoleteFactor", 1.5, ctrl_obsoleteFactor_);
	setctrlState("mrs_real/obsoleteFactor", true);
	addctrl("mrs_real/childrenScoreFactor", 0.01, ctrl_childrenScoreFactor_);
	setctrlState("mrs_real/childrenScoreFactor", true);
	addctrl("mrs_real/bestFactor", 1.1, ctrl_bestFactor_);
	setctrlState("mrs_real/bestFactor", true);
	addctrl("mrs_natural/eqPhase", 2, ctrl_eqPhase_);
	setctrlState("mrs_natural/eqPhase", true);
	addctrl("mrs_natural/eqPeriod", 1, ctrl_eqPeriod_);
	setctrlState("mrs_natural/eqPeriod", true);
	addctrl("mrs_real/corFactor", 0.25, ctrl_corFactor_);
	setctrlState("mrs_real/corFactor", true);
	addctrl("mrs_real/child1Factor", 2.0, ctrl_child1Factor_);
	setctrlState("mrs_real/child1Factor", true);
	addctrl("mrs_real/child2Factor", 0.5, ctrl_child2Factor_);
	setctrlState("mrs_real/child2Factor", true);
	addctrl("mrs_real/child3Factor", 1.0, ctrl_child3Factor_);
	setctrlState("mrs_real/child3Factor", true);
	addctrl("mrs_natural/metricalChangeTime", 0, ctrl_metricalChangeTime_);
	setctrlState("mrs_natural/metricalChangeTime", true);
	addctrl("mrs_bool/backtrace", false, ctrl_backtrace_);
	setctrlState("mrs_bool/backtrace", true);
}

void
BeatReferee::myUpdate(MarControlPtr sender)
{
	MRSDIAG("BeatReferee.cpp - BeatReferee:myUpdate");
	
	ctrl_onSamples_->setValue(1, NOUPDATE);
	ctrl_onObservations_->setValue(1, NOUPDATE);
	ctrl_osrate_->setValue(ctrl_israte_, NOUPDATE);

	obsoleteFactor_ = ctrl_obsoleteFactor_->to<mrs_real>();
	childrenScoreFactor_ = ctrl_childrenScoreFactor_->to<mrs_real>();
	bestFactor_ = ctrl_bestFactor_->to<mrs_real>();
	eqPhase_ = ctrl_eqPhase_->to<mrs_natural>();
	eqPeriod_ = ctrl_eqPeriod_->to<mrs_natural>();
	corFactor_ = ctrl_corFactor_->to<mrs_real>();
	child1Factor_ = ctrl_child1Factor_->to<mrs_real>();
	child2Factor_ = ctrl_child2Factor_->to<mrs_real>();
	child3Factor_ = ctrl_child3Factor_->to<mrs_real>();
	metricalChangeTime_ = ctrl_metricalChangeTime_->to<mrs_natural>();
	backtrace_ = ctrl_backtrace_->to<mrs_bool>();

	//inObservations_ = number of BeatAgents in the pool
	nrAgents_ = inObservations_;	
	historyCount_.create(nrAgents_); //1index for each agent
	
	//historyBeatTimes_.create(nrAgents_,1000); //1index for each agent
	//statsPeriods_.create(nrAgents_,10000); //1index for each agent
	//statsPhases_.create(nrAgents_,10000); //1index for each agent
	//statsAgentsLifeCycle_.create(nrAgents_,10000);
	//statsAgentsScore_.create(nrAgents_,10000);
	//statsMuted_.create(nrAgents_, 10000);
	
	score_.create(nrAgents_); //1index for each agent
	lastPeriods_.create(nrAgents_); //1index for each agent
	lastPhases_.create(nrAgents_); //1index for each agent
	mutedAgents_.create(nrAgents_);//1index for each agent
	beatCounter_.create(nrAgents_);//1index for each agent
	initPeriod_.create(nrAgents_);//1index for each agent

	//Agent control indexed matrix
	//Each line(observation) accounts for an agent
	//[New/Update_Flag|Period|Phase|Timming]
	agentControl_.create(nrAgents_, 4);
	updctrl(ctrl_agentControl_, agentControl_);

	for(int i = 0; i < nrAgents_; i++)
	{
		historyCount_(i) = 1.0;
		score_(i) = 0.0;
		lastPeriods_(i) = 0.0;
		lastPhases_(i) = 0.0;
		mutedAgents_(0, i) = 1.0;
		beatCounter_(i) = 0.0;
		initPeriod_(i) = 0.0;
	}

	inductionEnabler_ = ctrl_inductionEnabler_->to<mrs_realvec>();

	//Calculate minimumPeriod (eq. considered possible maximumTempo)
	hopSize_ = ctrl_hopSize_->to<mrs_natural>();
	srcFs_ = ctrl_srcFs_->to<mrs_real>();

	mrs_natural maxTempo = ctrl_maxTempo_->to<mrs_natural>();
	mrs_natural minTempo = ctrl_minTempo_->to<mrs_natural>();
	minPeriod_ = (mrs_natural) floor(60.0 / (maxTempo * hopSize_) * srcFs_);
	maxPeriod_ = (mrs_natural) ceil(60.0 / (minTempo * hopSize_) * srcFs_);

	//for disabling metrical changes -> which may represent limiting period changes to a maximum of 20BPMs)
	maxPeriodChange_ = (60.0/20)*(srcFs_/hopSize_); //[!! 20BPMs meaninghless but no easy way of restricting metrical changes !!]
	timeBeforeKilling_ = (mrs_natural)(5.0*srcFs_/hopSize_); //wait 5secs before considering killing obsolete agents
}
//Routine for calculating the first beat time of the first created agents (after induction):
//(for continuous signal - no backtracing (induction window offset -> first beat must occur after the induction stage)):
mrs_natural
BeatReferee::calculateFirstBeat(mrs_natural initPeriod, mrs_natural initPhase)
{
	mrs_natural count = (mrs_natural) ceil((inductionTime_ - initPhase) /  (mrs_real)initPeriod);
	mrs_natural firstBeat = (initPhase + initPeriod * count);
	
	//Due to the compensation bellow we must grant that nextBeat is bigger than inductionTime_
	//If it is equal than we must postpone the nextBeat to a period after
	if(firstBeat == inductionTime_)
	{
		firstBeat += initPeriod;
	}

	//cout << "initPeriod:" << initPeriod << "; initPhase:" << initPhase << "; firstBeat:" << firstBeat << endl;

	return firstBeat;
}

mrs_natural
BeatReferee::getFirstAliveAgent()
{
	mrs_natural firstAlive = 0;
	for(mrs_natural a = 0; a < nrAgents_; a++)
	{
		if(!mutedAgents_(a))
		{
			firstAlive = a;
			break;
		}
	}
	return firstAlive;
}

//Get the current worst agent of the pool:
mrs_natural
BeatReferee::getWorstAgent()
{
	//By default lowest score = score from first agent
	mrs_natural firstAlive= getFirstAliveAgent();
	mrs_real lowestScore = score_(firstAlive);
	mrs_natural lowestIndex = firstAlive;
	
	for(mrs_natural a = firstAlive; a < nrAgents_; a++)
	{
		if(score_(a) < lowestScore)
		{
			lowestScore = score_(a);
			lowestIndex = a;
		}
	}

	//return worst agent
	return lowestIndex;
}

mrs_realvec 
BeatReferee::calculateNewHypothesis(mrs_natural agentIndex, mrs_natural oldPeriod, mrs_natural prevBeat, mrs_natural error)
{
	mrs_natural newPeriod;
	mrs_natural nextBeat;
	newPeriod =  oldPeriod + ((mrs_natural) ((error*corFactor_) + ((error/abs(error)) * 0.5)));

	//To avoid too small or big periods, or too distanced from agent's initial period:
	if(newPeriod > minPeriod_ && newPeriod < maxPeriod_ && 
		fabs(initPeriod_(agentIndex) - newPeriod) < 0.1*initPeriod_(agentIndex))
	{		
		nextBeat = prevBeat + newPeriod + ((mrs_natural) ((error*corFactor_) + ((error/abs(error))) * 0.5));
	}

	else 
	{
		nextBeat = prevBeat + oldPeriod;
		newPeriod = oldPeriod;
	}

	//if(agentIndex == bestAgentIndex_)
	//	cout << "Agent" << agentIndex << ": Error:" << error << "; AgentPeriod:" << newPeriod << endl;

	//cout << "Agent " << agentIndex << "; oldPeriod: " << oldPeriod << "; NewPeriod: " << newPeriod <<
	//	"; NextBeat: "  << nextBeat << "; Error: " << error << "; Correction: " << correction << endl;
	//cout << "Agent " << agentIndex << " History: " << historyCount_(agentIndex) << endl;

	mrs_realvec newHypothesis(2);
	newHypothesis(0) = newPeriod;
	newHypothesis(1) = nextBeat;

	return newHypothesis;
}

mrs_natural
BeatReferee::calcNewPeriod(mrs_natural oldPeriod, mrs_natural error, mrs_real beta)
{
	//cout << "error: " << error << "; beta: " << beta << endl;
	mrs_natural newPeriod = oldPeriod + ((mrs_natural) ((error * beta) + (error/abs(error)) * 0.5));
	if(newPeriod < minPeriod_ || newPeriod > maxPeriod_)
		newPeriod = oldPeriod;

	//cout << "oldPeriod: " << oldPeriod << "; newPeriod: " << newPeriod << "; Error: " << error << 
	//	"; Beta: " << beta << endl;

	return newPeriod;
}

mrs_realvec 
BeatReferee::calcChildrenHypothesis(mrs_natural oldPeriod, mrs_natural prevBeat, mrs_natural error)
{
	mrs_natural nextBeat;
	mrs_natural newPeriod;
	mrs_realvec newHypotheses(6); //for 3 children

	newPeriod = oldPeriod;
	
	//cout << "ChildFactor1:" << child1Factor_ << "; ChildFactor2:" << child2Factor_ << "; ChildFactor3:" << child3Factor_ << endl;

	//(if childiFactor = -1.0 then only phase adjusted)
	if(child1Factor_ == 2.0)
		nextBeat = prevBeat + newPeriod + error;	
	else
	{
		newPeriod = calcNewPeriod(oldPeriod, error, child1Factor_);
		nextBeat = prevBeat + newPeriod + ((mrs_natural) ((error*child1Factor_) + ((error/abs(error))) * 0.5));
	}
	newHypotheses(0) = newPeriod;
	newHypotheses(1) = nextBeat;
	
	if(child2Factor_ == 2.0)
		nextBeat = prevBeat + newPeriod + error;	
	else
	{
		newPeriod = calcNewPeriod(oldPeriod, error, child2Factor_);
		nextBeat = prevBeat + newPeriod + ((mrs_natural) ((error*child2Factor_) + ((error/abs(error))) * 0.5));
	}
	newHypotheses(2) = newPeriod;
	newHypotheses(3) = nextBeat;

	if(child3Factor_ == 2.0)
		nextBeat = prevBeat + newPeriod + error;	
	else
	{
		newPeriod = calcNewPeriod(oldPeriod, error, child3Factor_);
		nextBeat = prevBeat + newPeriod + ((mrs_natural) ((error*child3Factor_) + ((error/abs(error))) * 0.5));
	}
	newHypotheses(4) = newPeriod;
	newHypotheses(5) = nextBeat;

	//cout << "oldPeriod: " << oldPeriod << "; newPeriod: " << newPeriod << "; Error: " << error << 
	//	"; prevBeat: " << prevBeat << "; nextBeat: " << nextBeat << endl;

	return newHypotheses;
}

//Routine for creating new agents from existent one
void
BeatReferee::createChildren(mrs_natural agentIndex, mrs_natural oldPeriod, mrs_natural prevBeat, mrs_natural error, 
						mrs_real agentScore, mrs_real beatCount)
{
	mrs_real deltaS = fabs(childrenScoreFactor_ * agentScore);
	mrs_real newScore = agentScore - deltaS;

	mrs_realvec newHypotheses = calcChildrenHypothesis(oldPeriod, prevBeat, error);

	//mrs_realvec newHypothesis = calculateNewHypothesis(agentIndex, oldPeriod, prevBeat, error);
	//setNewHypothesis(agentIndex, (mrs_natural) newHypothesis(0), (mrs_natural) newHypothesis(1));

	if(child1Factor_ != -1.0)
		createNewAgent((mrs_natural) newHypotheses(0), (mrs_natural) newHypotheses(1), newScore, beatCount);
	if(child1Factor_ != -1.0)
		createNewAgent((mrs_natural) newHypotheses(2), (mrs_natural) newHypotheses(3), newScore, beatCount);
	if(child1Factor_ != -1.0)
		createNewAgent((mrs_natural) newHypotheses(4), (mrs_natural) newHypotheses(5),  newScore, beatCount);

	//Display Created BeatAgent:
	//cout << "NEW AGENT(" << t_ << "-" << ((t_ * hopSize_) - (hopSize_/2)) / srcFs_ << ") (reqBy:" << agentIndex << 
	//") -> PrevBeat:" << prevBeat << " Period:" << oldPeriod << " NextBeat1:" << newHypotheses(1) << " NewPeriod1:" << 
	//	newHypotheses(0) << " NextBeat2:" << newHypotheses(3) << " NewPeriod2:" << newHypotheses(2) << 
	//	" NextBeat3:" << newHypotheses(5) << " NewPeriod3:" << newHypotheses(4) << 
	//	" Error:" << error << " Score:" << newScore << endl;					
}

//Routine for updating existent agent hypothesis
//(Used when beat of agent is found inside its inner tolerance with some error):
void
BeatReferee::updateAgentHypothesis(mrs_natural agentIndex, mrs_natural oldPeriod, 
								   mrs_natural prevBeat, mrs_natural error)
{
	mrs_realvec newHypothesis = calculateNewHypothesis(agentIndex, oldPeriod, prevBeat, error);
	setNewHypothesis(agentIndex, (mrs_natural) newHypothesis(0), (mrs_natural) newHypothesis(1));

	//Display Updated BeatAgent:
	//cout << "UPDATING AGENT" << agentIndex <<" (" << t_ << ")" << " -> oldPeriod: " << oldPeriod << 
	//	" newPeriod: " << newHypothesis(0) << " prevBeat: " << prevBeat << " nextBeat: " << newHypothesis(1) <<  
	//	" Error: " << error << endl;
}

//Define new Hypothesis in indexed AgentControl Matrix:
void
BeatReferee::setNewHypothesis(mrs_natural agentIndex, mrs_natural newPeriod, mrs_natural nextBeat)
{
	agentControl_(agentIndex, 0) = 1.0; //is New or Updated
	agentControl_(agentIndex, 1) = newPeriod;
	agentControl_(agentIndex, 2) = nextBeat;
	agentControl_(agentIndex, 3) = t_;

	updctrl(ctrl_agentControl_, agentControl_);

	lastPeriods_(agentIndex) = newPeriod;
}

void
BeatReferee::grantPoolSpace()
{
	mrs_bool isAvailable = false;
	
	for(int a = 0; a < mutedAgents_.getSize(); a++)
	{
		if(mutedAgents_(a))
		{
			isAvailable = true;
			break;
		}
	}

	//if there are no free agents -> remove worst!
	if(!isAvailable)
	{
		mrs_natural agentInd2Kill = getWorstAgent();
		killAgent(agentInd2Kill, "Pool");
	}
}

//Generic routine for creating new agents given their hypotheses:
void
BeatReferee::createNewAgent(mrs_natural newPeriod, mrs_natural firstBeat, mrs_real newScore, mrs_real beatCount)
{
	//Grant available space in the pool, by removing the worst agent, if needed
	grantPoolSpace();

	for(int a = 0; a < mutedAgents_.getSize(); a++)
	{
		//Look for first disabled agent:		
		if(mutedAgents_(a))
		{
			//Activate new agent
			mutedAgents_(a) = 0.0;
			updctrl(ctrl_mutedAgents_, mutedAgents_);

			//Diplay Created BeatAgent:
			//cout << " CREATED: Agent " << a << endl;

			//Defines new hypothesis for this agent:
			setNewHypothesis(a, newPeriod, firstBeat);
			
			//Update score:
			score_(a) =  newScore;
			beatCounter_(a) = beatCount;

			//Update Agents' Periods and Phases (for equality checking)
			lastPeriods_(a) = newPeriod; //(Periods in frames)
			lastPhases_(a) = firstBeat; //(Phases in frames)
			
			initPeriod_(a) = newPeriod; //save agent's initial IBI

			//statsPeriods_(a, t_) = newPeriod; 
			//statsPhases_(a, t_) = firstBeat;

			//statsAgentsLifeCycle_(a, t_) = 1.0;
			
			break;
		}
	}
}

//Generic routine for killing agents:
void
BeatReferee::killAgent(mrs_natural agentIndex, mrs_string motif)
{
	//Never kill a best agent (for increasing inertia) -> a best agent must live until being replaced by a better one
	if(agentIndex != bestAgentIndex_)
	{		
		//Diplay killed BeatAgent:
		//cout << "KILLED AGENT " << agentIndex << " (" << motif << ") With Score: " << score_(agentIndex) << " / " << bestScore_ << endl;

		mutedAgents_(agentIndex) = 1.0;
		updctrl(ctrl_mutedAgents_, mutedAgents_);
		score_(agentIndex) = 0.0;
		beatCounter_(agentIndex) = 0.0;
		historyCount_(agentIndex) = 1.0;

		//statsAgentsLifeCycle_(agentIndex, t_) = -1;
		lastPeriods_(agentIndex) = 0; //(Periods in frames)
		lastPhases_(agentIndex) = 0; //(Phases in frames)
	}
}

void
BeatReferee::calcAbsoluteBestScore()
{
	for (mrs_natural o = 0; o < nrAgents_; o++)
	{
		//Only consider alive agents:
		if(!mutedAgents_(o))
		{
			if(t_ <= metricalChangeTime_ || (t_ > metricalChangeTime_ && 
				fabs((1/lastPeriods_(bestAgentIndex_))-(1/lastPeriods_(o))) <= (1/maxPeriodChange_)))
			{
				if((bestScore_ >= 0 && score_(o) >= bestFactor_ * bestScore_) || 
					(bestScore_ < 0 && score_(o) >= bestScore_ / bestFactor_))
				{
					bestScore_ = score_(o);
					bestAgentIndex_ = o;
				}
			}
		}
	}

	//cout << "Absolute Best Agent: " << bestAgentIndex_ << " BestScore: " << bestScore_ << endl;
}

void 
BeatReferee::myProcess(realvec& in, realvec& out)
{
	//Inititally desactivate all agents:
	if(t_ == 0)
		updctrl(ctrl_mutedAgents_, mutedAgents_);

	//While no best beat detected => outputs 0 (no beat)
	out.setval(0.0);
	ctrl_beatDetected_->setValue(0.0);

	t_++;
	//pass value to beatSink (if used)
	ctrl_tickCount_->setValue(t_);
	
	agentControl_ = ctrl_agentControl_->to<mrs_realvec>();
	//Always updates agent's timming to equalize referee's:
	for(mrs_natural i = 0; i < agentControl_.getRows(); i++)
	{
		agentControl_(i, 3) = t_;
		updctrl(ctrl_agentControl_, agentControl_);
	}

	//Display Input from BeatAgents:
	//cout << "INPUT (" << t_ << "): ";
/*	for(mrs_natural a = 0; a < nrAgents_; a++)
		cout << "\n" << a << "-> " << in(a, 0) << " | " << in(a, 1) << " | " << in(a, 2) << " | " << in(a, 3); 
	cout << endl;
*/

	//realvec with the enable flag of all the BeatAgents in the pool
	//(0 -> agent active; 1 -> agent desactivated)
	mutedAgents_ = ctrl_mutedAgents_->to<mrs_realvec>();

	//cout << "Beat1: " << firstHypotheses_(0,0) << " BPM1: " << firstHypotheses_(0,1) 
	//	<< " Beat2: " << firstHypotheses_(1,0) << " BPM2: " << firstHypotheses_(1,1) 
	//		<< " Beat3: " << firstHypotheses_(2,0) << " BPM3: " << firstHypotheses_(2,1) << endl;

	//Create the first BeatAgents with new hypotheses just after Tseconds of induction:
	//(new agents' score shall be the average of all the already existent ones)
	inductionTime_ = ctrl_inductionTime_->to<mrs_natural>();
	if(t_ == inductionTime_ && !inductionFinnished_)
	{
		firstHypotheses_ = ctrl_firstHypotheses_->to<mrs_realvec>();

		mrs_natural newAgentPeriod;
		mrs_natural newAgentPhase;
		mrs_real newAgentScore;

		//Updating initial bestScore and creating first agents:
		//(nr of initial agents equals nr of bpm hypotheses)
		for(mrs_natural i = 0; i < firstHypotheses_.getRows(); i++)
		{
			if((mrs_natural) firstHypotheses_(i,0) > 0) //only consider valid hypothesis:
			{
				//firstHypotheses_ -> matrix with i generated beat hypotheses + score, in the induction stage
				//[ BPMi | Beati | Score i ]
				newAgentPeriod = (mrs_natural) firstHypotheses_(i,0);
				newAgentPhase = (mrs_natural) firstHypotheses_(i,1);
				newAgentScore = firstHypotheses_(i,2);

				if(newAgentScore > bestScore_)
				{
					bestScore_ = newAgentScore;
					bestAgentIndex_ = i;
					//cout << "Best Score From: " << i << "(" << bestScore_ << ")" << endl;
				}

				//multiplied by sqrt(period) for disinflating the faster agents (with smaller periods)
				//newAgentScore *= sqrt((mrs_real)maxPeriod_);
				if(backtrace_) //if backtrace mode the first beat will be considered in a 0 offset
					createNewAgent(newAgentPeriod, newAgentPhase, newAgentScore, 0);
				else //if not the first beat must occur after the induction window (induction offset)
					createNewAgent(newAgentPeriod, calculateFirstBeat(newAgentPeriod, newAgentPhase), newAgentScore, 0);
				
				//cout << "Score" << i << ": " << newAgentScore << endl;
				
				if(i == nrAgents_-1)
				{
					MRSWARN("Last hypotheses discarted because the nr. of hypotheses surpasses the nr. of BeatAgents");
					break;
				}

				if(newAgentPeriod == 0)
				{
					MRSWARN("Last hypotheses discarted because no more periods considered");
					break;
				}
			}
		}

		//multiplied by sqrt(period) for disinflating the faster agents (with smaller periods)
		//bestScore_ *= sqrt((mrs_real)maxPeriod_);

		//After finnishing induction disable induction functioning (tempoinduction Fanout):
		for(mrs_natural i = 0; i < inductionEnabler_.getSize(); i++)
			inductionEnabler_(0, i) = 1.0; //diable = muted
			
		updctrl(ctrl_inductionEnabler_, inductionEnabler_);
		
		//if backtrace restart tick counter
		if(backtrace_)
			t_ = 0;
		
		inductionFinnished_ = true;
	}
	
	//After induction:
	if(inductionFinnished_)
	{
		//INPUT: [Beat/Eval/None|Period|PrevBeat|Inner/Outter|Error|Score] -> inSamples_ = 6
		mrs_real agentFlag;
		mrs_natural agentPeriod;
		mrs_natural agentPrevBeat;
		mrs_real agentTolerance;
		mrs_natural agentError;
		mrs_real agentDScore;

		//Process all the agent pool for score, tempo and phase updates:
		for (mrs_natural o = 0; o < nrAgents_; o++)
		{
			agentFlag = in(o, 0);

			//statsPeriods_(o, t_) = lastPeriods_(o); 
			//statsPhases_(o, t_) = lastPhases_(o); 
			//statsAgentsScore_(o, t_) = score_(o);
			//statsMuted_(o, t_) = mutedAgents_(o);

			//Only consider alive agents that send new evaluation:
			//(Remind that each ouputed beat by the agents is only
			//evaluated at the end of its beat position + outterWindow tolerance)
			if(!mutedAgents_(o) && agentFlag == EVAL)
			{	
				agentDScore = in(o, 5);
				agentPeriod = (mrs_natural) in(o, 1);
				agentPrevBeat = (mrs_natural) in(o, 2);

				//Update Agents' Periods and Phases (for equality checking)
				lastPeriods_(o) = agentPeriod; //(Periods in frames)
				lastPhases_(o) = agentPrevBeat; //(Phases in frames)

				//statsPeriods_(o, t_) = agentPeriod; 
				//statsPhases_(o, t_) = agentPrevBeat; 

				//Update Agents' Score
				score_(o) += agentDScore;

				//If the bestAgent drops or increases its score the BestScore has to drop correspondingly
				if((score_(bestAgentIndex_) < bestScore_) || (score_(bestAgentIndex_) > bestScore_))
				{
					//cout << "Updating bestScore: " << "OLD: " << bestScore_ << " NEW: " << score_(bestAgentIndex_) << endl;
					bestScore_ = score_(bestAgentIndex_);
					calcAbsoluteBestScore();
				}

				//Updating bestScore:
				if(t_ <= metricalChangeTime_ || (t_ > metricalChangeTime_ && 
					fabs((1/lastPeriods_(bestAgentIndex_))-(1/lastPeriods_(o))) <= (1/maxPeriodChange_)))
				{
					if((bestScore_ >= 0 && score_(o) >= bestFactor_ * bestScore_) || 
						(bestScore_ < 0 && score_(o) >= bestScore_ / bestFactor_))
					{
						//cout << "bestScore_OLD: " << bestScore_ << " NEW_bestScore: " << score_(o) << endl;
						bestScore_ = score_(o);
						bestAgentIndex_ = o;
					}
				}

				
				//if(o == bestAgentIndex_)
				//	cout << "Agent" << o << "-dScore:" << agentDScore << "; agentScore:" << score_(o) 
				//	<< "(" << bestScore_ << ")" << endl;

				//Kill Agent if its score is bellow minimum (wait 5seconds before taking it into consideration)
				if (t_ > timeBeforeKilling_ && score_(o) < bestScore_ && fabs(bestScore_ - score_(o)) > fabs(bestScore_ * obsoleteFactor_))
				{
					//cout << "Agent " << o << " Killed: Score below minimum (" << score_(o) << "\\" << bestScore_ << ")" << endl;
					killAgent(o, "Obsolete");
				}
				
				//statsAgentsScore_(o, t_) = score_(o);

			//Display Scores from BeatAgents:
			/*	cout << "SCORES(" << t_ << ") (reqBy:" << o << "): " << endl;
				for(mrs_natural a = 0; a < nrAgents_; a++)
					cout << a << "-> " << score_(a) << " ";
				cout << endl;
			*/
			}
		}

		//Process all the agent pool for testing best beat and consider
		//the generation of new beats or the killing of existent ones
		for (mrs_natural o = 0; o < nrAgents_; o++)
		{
			agentFlag = in(o, 0);

			//Only consider alive agents that send new evaluation:
			if(!mutedAgents_(o) && agentFlag == EVAL)
			{
				
				agentPeriod = (mrs_natural) in(o, 1);
				agentPrevBeat = (mrs_natural) in(o, 2);
				agentTolerance = in(o, 3);
				agentError = (mrs_natural) in(o, 4);
				agentDScore = in(o, 5);

				//If a beat of an agent is inside its Inner tolerance but it has an error:
				//Update agent phase and period hypotheses:
				if(agentTolerance == INNER && abs(agentError) > 0)
				{
					updateAgentHypothesis(o, agentPeriod, agentPrevBeat, agentError);
				}
				
				//If a beat of an agent is detected outside its Inner tolerance window:
				//Is created another agent that keeps this agent hypothesis, and updates its phase
				//and period to account for the given error:
				if(agentTolerance == OUTTER)
				{	
					//CREATE NEW AGENT WITH THIS NEW HYPOTHESIS KEEPING THE SCORE OF o-delta:
					//(the agent that generates a new one keeps its original hypothesis);
					//New agent must look for a new beat on the next (updated) period
					createChildren(o, agentPeriod, agentPrevBeat, agentError, score_(o), beatCounter_(o));
					//statsAgentsLifeCycle_(o, t_) = 2;
				}
				
				//Checks if there are 2 equal agents
				//2 agents are considered equal if their periods don't differ 
				//more than 10ms (eq. 1frame) and their phases no more than 20ms (eq. 2frames)				
				for(mrs_natural oo = 0; oo < nrAgents_; oo++)
				{
					if(oo != o)
					{						
						//if((abs(agentPeriod - lastPeriods_(oo)) < eqPeriod_) && (abs(agentPrevBeat - lastPhases_(oo) < eqPhase_)))
						if((abs(agentPeriod - in(oo, 1)) < eqPeriod_) && (abs(agentPrevBeat - in(oo, 2) < eqPhase_)))
						{
							//From the two equal agents kill the one with lower score (if it isn't the current best agent)
							ostringstream motif;
							if(score_(o) >= score_(oo))
							{
								if(oo != bestAgentIndex_) 
								{
									//cout << "1-KILL Agent " << oo << " (" << score_(oo) << ") EQUAL TO Agent " << o << " (" << score_(o) << ")" << endl;
									motif << "Equal to " << o;
									killAgent(oo, motif.str());
								}
								else 
								{
									//cout << "2-KILL Agent " << o << " (" << score_(o) << ") EQUAL TO Agent " << oo << " (" << score_(oo) << ")" << endl;
									motif << "Equal to " << oo;
									killAgent(o, motif.str());
									break; //in this case breaks because considered agent no longer exists.
								}
							}
							else
							{
								if(o != bestAgentIndex_) 
								{
									//cout << "3-KILL Agent " << o << " (" << score_(o) << ") EQUAL TO Agent " << oo << " (" << score_(oo) << ")" << endl;
									motif << "Equal to " << oo;
									killAgent(o, motif.str());
									break; //in this case breaks because considered agent no longer exists.
								}
								else 
								{
									motif << "Equal to " << o;
									killAgent(oo, motif.str());
									//cout << "1-KILL Agent " << oo << " (" << score_(oo) << ") EQUAL TO Agent " << o << " (" << score_(o) << ")" << endl;
								}
							}
						}
					}
				}
				
			}
			
			if(!mutedAgents_(o) && agentFlag == BEAT)
			{
				//Increment beat counter of each agent
				beatCounter_(o)++;

				//Display Beats from BeatAgents:
				//cout << "Agent " << o << "(" << t_ << ") -> AgentScore: " << score_(o) << " BestAgent: " 
				//	<< bestAgentIndex_ << " BestScore: " << bestScore_ << " BeatCount: " << beatCounter_(o) << endl;
				
				//if best agent sends beat => outputs best beat
				if(o == bestAgentIndex_)
				{	
					//to avoid beats given by different agents distanced less then minPeriod frames (+ a tolerance = -3)
					if(t_ - lastBeatTime_ >= (minPeriod_-3))
					{
						//Display Outputted Beat:
						//cout << "OUTPUT(" << t_ << "-" << ((t_ * hopSize_) - (hopSize_/2)) / srcFs_ << "):Beat from Agent " << bestAgentIndex_ << 
						//	" BestScore:" << bestScore_ << " (" << score_(bestAgentIndex_) << ")";
						//cout << ":" << (60.0 / (t_ - lastBeatTime_)) * (ctrl_srcFs_->to<mrs_real>() / ctrl_hopSize_->to<mrs_natural>()) << "BPM" << endl;
						//cout << "BEST_AgentPeriod: " << lastPeriods_(bestAgentIndex_) << "(" << (t_ - lastBeatTime_) << ")" << endl;
						
						ctrl_beatDetected_->setValue(1.0);
						out.setval(1.0);
						//Updates agent history, which accounts for the total number
						//of the detected best (considered) beats of each agent:
						historyCount_(o)++;
						//historyBeatTimes_(o, outputCount_) = t_;
						outputCount_ ++;
						lastBeatTime_ = t_;
					}				
				}
			}
		}
	}


	//MATLAB_PUT(in, "BeatAgents");
	/*
	MATLAB_PUT(out, "BeatReferee");
	MATLAB_PUT(historyBeatTimes_, "HistoryBeatTimes");
	MATLAB_PUT(historyCount_, "HistoryCount");
	MATLAB_PUT(statsPeriods_, "statsPeriods");
	MATLAB_PUT(statsAgentsLifeCycle_, "statsAgentsLifeCycle");
	MATLAB_PUT(statsAgentsScore_, "agentsScore");
	MATLAB_PUT(statsMuted_, "mutedAgents");
	MATLAB_PUT(bestScore_, "bestScore");
	MATLAB_EVAL("bestAgentScore = [bestAgentScore bestScore];");
	MATLAB_EVAL("FinalBeats = [FinalBeats, BeatReferee];");
	*/
	//MATLAB_EVAL("hold on;");
	//MATLAB_EVAL("plot(BeatAgentsTS)");
	//MATLAB_EVAL("stem(t, 1, 'r');");
	//MATLAB_EVAL("hold off;");
}