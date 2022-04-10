#pragma once

namespace FinalComputerVisionProject {
	enum SEType{
		SQUARE,
		CROSS
	};

	struct StructuringElement{
		SEType type;
		uint8_t size;
	};
}
