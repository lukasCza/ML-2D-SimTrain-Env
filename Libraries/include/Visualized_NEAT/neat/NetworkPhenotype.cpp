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

#include <Visualized_NEAT/neat/NetworkPhenotype.h>

#include <assert.h>

#include <iostream>

using namespace neat;

NetworkPhenotype::NetworkPhenotype()
: _activationMultiplier(4.0f)
{}

NeuronInput &NetworkPhenotype::getNeuronInputNode(size_t index) {
	if (index >= _inputs.size()) {
		const size_t numHiddenAndInput = _inputs.size() + _hidden.size();

		if (index >= numHiddenAndInput)
			return _outputs[index - numHiddenAndInput];

		return _hidden[index - _inputs.size()];
	}

	return _inputs[index];
}

Neuron &NetworkPhenotype::getNeuronNode(size_t index) {
	assert(index >= _inputs.size());

	const size_t numHiddenAndInput = _inputs.size() + _hidden.size();

	if (index >= numHiddenAndInput)
		return _outputs[index - numHiddenAndInput];

	return _hidden[index - _inputs.size()];
}

void NetworkPhenotype::create(size_t numInputs, size_t numOutputs, const std::vector<std::shared_ptr<ConnectionGene>> &connections, size_t numHidden) {
	// Clear existing data, if there is any
	_inputs.clear();
	_hidden.clear();
	_outputs.clear();

	// Create neurons and neuron inputs
	_inputs.resize(numInputs);
	_hidden.resize(numHidden);
	_outputs.resize(numOutputs);

	const size_t numUnits = numInputs + numHidden + numOutputs;

	// Connect neurons
	for (size_t i = 0, size = connections.size(); i < size; i++) {
		if (!connections[i]->_enabled) // Skip disabled genes
			continue;

		// Cannot have an output to a input
		if (connections[i]->_outIndex < _inputs.size() || connections[i]->_outIndex >= numUnits)
			continue;

		Neuron::Synapse newSynapse;
		newSynapse._inputOffset = connections[i]->_inIndex;
		newSynapse._weight = connections[i]->_weight;

		getNeuronNode(connections[i]->_outIndex)._inputs.push_back(newSynapse);
	}
}

void NetworkPhenotype::create(const NetworkGenotype &genotype) {
	create(genotype.getNumInputs(), genotype.getNumOutputs(), genotype.getConnectionSet(), genotype.getNumHidden());
}

void NetworkPhenotype::update() {
	for (size_t i = 0, size = _hidden.size(); i < size; i++)
		_hidden[i].update(*this);

	for (size_t i = 0, size = _outputs.size(); i < size; i++)
		_outputs[i].update(*this);
}

void NetworkPhenotype::resetOutputs() {
	for (size_t i = 0, size = _hidden.size(); i < size; i++)
		_hidden[i]._output = 0.0f;

	for (size_t i = 0, size = _outputs.size(); i < size; i++)
		_outputs[i]._output = 0.0f;
}