/*
	NEAT Visualizer
	Copyright (C) 2012-2014 Eric Laukien

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
		claim that you wrote the original software. If you use this software
		in a product, an acknowledgment in the product documentation would be
		appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
		misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include <Visualized_NEAT/neat/NeuronInput.h>

#include <vector>
#include <ostream>
#include <assert.h>

namespace neat {
	class Neuron : public NeuronInput {
	public:
		struct Synapse {
			size_t _inputOffset;

			float _weight;

			Synapse()
				: _inputOffset(0), _weight(0.0f)
			{}
		};

		std::vector<Synapse> _inputs;

		float _bias;

		Neuron();

		// Inherited from NeuronInput
		virtual void update(class NetworkPhenotype &phenotype);
	};

	std::ostream &operator<<(std::ostream &os, Neuron &neuron);
}