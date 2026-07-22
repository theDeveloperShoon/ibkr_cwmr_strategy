#include "Utility.h"

#include <array>
#include <string>
#include <sstream>
#include <vector>

static const std::string base64Chars =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

std::string Utility::doubleMaxString(double d, std::string def) {
	if (d == DBL_MAX) {
		return def;
	}
	else {
		std::ostringstream oss;
		oss.precision(8);
		oss << std::fixed << d;
		std::string str = oss.str();
		std::size_t pos1 = str.find_last_not_of('0');
		if (pos1 != std::string::npos) {
			str.erase(pos1 + 1);
		}
		std::size_t pos2 = str.find_last_not_of('.');
		if (pos2 != std::string::npos) {
			str.erase(pos2 + 1);
		}

		return str;
	}
}

std::string Utility::doubleMaxString(double d) {
	return doubleMaxString(d, "");
}

std::string Utility::intMaxString(int n) {
	return (n == INT_MAX) ? "" : std::to_string(n);
}

std::string Utility::llongMaxString(long long n) {
	return (n == LLONG_MAX) ? "" : std::to_string(n);
}

std::string Utility::getFundDistributionPolicyIndicatorString(FundDistributionPolicyIndicator fundDistributionPolicyIndicator) {
	switch (fundDistributionPolicyIndicator) {
	case FundDistributionPolicyIndicator::None:
		return "None";
	case FundDistributionPolicyIndicator::AccumulationFund:
		return "AccumulationFund";
	case FundDistributionPolicyIndicator::IncomeFund:
		return "IncomeFund";
	default:
		return "Unknown";
	}
}

std::string Utility::getFundAssetTypeName(FundAssetType fundAssetType) {
	switch (fundAssetType) {
	case FundAssetType::None:
		return "None";
	case FundAssetType::Others:
		return "Others";
	case FundAssetType::MoneyMarket:
		return "MoneyMarket";
	case FundAssetType::FixedIncome:
		return "FixedIncome";
	case FundAssetType::MultiAsset:
		return "MultiAsset";
	case FundAssetType::Equity:
		return "Equity";
	case FundAssetType::Sector:
		return "Sector";
	case FundAssetType::Guaranteed:
		return "Guaranteed";
	case FundAssetType::Alternative:
		return "Alternative";
	default:
		return "Unknown";
	}
}

std::string Utility::getOptionExerciseTypeName(OptionExerciseType optionExerciseType) {
	switch (optionExerciseType) {
	case OptionExerciseType::None:
		return "None";
	case OptionExerciseType::Exercise:
		return "Exercise";
	case OptionExerciseType::Lapse:
		return "Lapse";
	case OptionExerciseType::DoNothing:
		return "DoNothing";
	case OptionExerciseType::Assigned:
		return "Assigned";
	case OptionExerciseType::AutoexerciseClearing:
		return "AutoexerciseClearing";
	case OptionExerciseType::Expired:
		return "Expired";
	case OptionExerciseType::Netting:
		return "Netting";
	case OptionExerciseType::AutoexerciseTrading:
		return "AutoexerciseTrading";
	default:
		return "Unknown";
	}
}

std::vector<std::uint8_t> Utility::base64Decode(const std::string& encodedString) {
	int i = 0;
	
	std::array<uint8_t, 4> char_array_4{};
	std::array<uint8_t, 3> char_array_3{};

	std::vector<uint8_t> decodedBytes{};

	decodedBytes.reserve((encodedString.size() * 3) / 4);

	if (encodedString.empty()) {
		return decodedBytes;
	}

	for (const char c : encodedString) {
		if (isspace(c)) continue; // Skip whitespace
		if (c == '=') break; // Stop at padding character
		if (!isBase64(static_cast<std::uint8_t>(c))) continue; // Skip invalid characters

		char_array_4[i++] = static_cast<std::uint8_t>(c);

		if (i == 4) {
			for (uint8_t& ch : char_array_4) {
				ch = static_cast<std::uint8_t>(base64Chars.find(static_cast<char>(ch)));
			}

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			decodedBytes.insert(decodedBytes.end(), char_array_3.begin(), char_array_3.end());
			i = 0;
			char_array_4.fill(0);
		}
	}

	if (i > 0)
	{
		std::fill(char_array_4.begin() + i, char_array_4.end(), 0);

		for (uint8_t& ch : char_array_4) {
			ch = static_cast<std::uint8_t>(base64Chars.find(static_cast<char>(ch)));
		}

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		decodedBytes.insert(decodedBytes.end(), char_array_3.begin(), char_array_3.begin() + (i - 1));
	}

	return decodedBytes;
}

bool Utility::isBase64(std::uint8_t c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}