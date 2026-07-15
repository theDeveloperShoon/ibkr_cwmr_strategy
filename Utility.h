#pragma once
#ifndef CWMR_STRATEGY_UTILITY_H
#define CWMR_STRATEGY_UTILITY_H

#include <string>
#include <limits>
#include <cmath>
#include "CommonDefs.h"

class Utility {
public:
    static std::string doubleMaxString(double d, std::string def);
    static std::string doubleMaxString(double d);
	static std::string intMaxString(int n);
	static std::string llongMaxString(long long n);

	static std::string getFundDistributionPolicyIndicatorString(FundDistributionPolicyIndicator fundDistributionPolicyIndicator);
	static std::string getFundAssetTypeName(FundAssetType fundAssetType);
	static std::string getOptionExerciseTypeName(OptionExerciseType optionExerciseType);
};

#endif // !CWMR_STRATEGY_UTILITY_H
