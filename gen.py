import json
import functools

def replaceTokens(string, map):
    for key in map:
        string = string.replace(key, map[key])
    
    string = string.replace("inf", "std::numeric_limits<double>::infinity()")
    return string

def parseData(data:str):
    # print(data)
    namespace_name = data["top"]["name"]
    # print(namespace_name)
    atomics = []
    for model in data["atomic"]:
        map = {}
        atomicTemplateFile = open("templates/atomic.cpp", "r")
        map["_HEADER_MARCO_"] = model["name"] + "_H"
        map["_MODEL_NAME_"] = model["name"]
        map["_PHASE_NAME_"] = model["name"] + "Phase"
        map["_STATE_NAME_"] = model["name"] + "State"
        map["_NAMESPACE_NAME_"] = namespace_name
        #############################################
        

        # ___________________________________________
        # _____Genenerate input and output ports_____
        portDecl = []
        portDef = []
        states = {}

        for e in model["inports"]:
            portDecl.append("Port<" + e["type"] + "> " + e["name"] + ";")
            portDef.append(e["name"] + " = " + "addInPort<" + e["type"] + ">(\"" + e["name"] + "\");")

        for e in model["outports"]:
            portDecl.append("Port<" + e["type"] + "> " + e["name"] + ";")
            portDef.append(e["name"] + " = " + "addOutPort<" + e["type"] + ">(\"" + e["name"] + "\");")

        map["_PORT_DECLERATIONS_"] = functools.reduce(lambda a,b : "\t" + a + "\n\t" + b, portDecl)
        map["_PORT_DEFINITIONS_"] = functools.reduce(lambda a,b : "\t\t" + a + "\n\t\t" + b, portDef)

        # ___________________________________________
        # _______Genenerate States Transitions_______

        for e in model["states"]:
            states[e] = model["states"][e]
    
        map["_PHASE_FIELDS_"] = functools.reduce(lambda a, b : "\t" + a + ",\n\t" + b, states.keys())
        
        internalTransition = 2 * "\t" + "switch(s.phase) {\n"
        for e in model["internal_transitions"]:
            internalTransition += (3 * "\t" + "case(" + e["curr_state"] + "):\n")
            internalTransition += (4 * "\t" + "s.phase = " + e["new_state"] + ";\n")
            internalTransition += (4 * "\t" + "s.sigma = " + states[e["new_state"]] + ";\n")
            
            internalTransition += 4 * "\t" + "break;\n"
        internalTransition += (2 * "\t" + "}")
        map["_INTERNAL_TRANSITION_"] = internalTransition

        #############################################
        external_transition = 2 * "\t"
        external_transition_per_ports = {}
        for e in model["external_transitions"]:
            if not external_transition_per_ports.get(e["port"]):
                external_transition_per_ports.update({e["port"] : [e]})
            else:
                external_transition_per_ports[e["port"]].append(e)
        
        for key in external_transition_per_ports:
            external_transition += "{0}if({1}->getBag().back()) {{\n".format(1*"\t", key)
            for e in external_transition_per_ports[key]:
                external_transition += "{0}if({1}->getBag().back() == {2} && s.phase == {3}) {{\n".format(4*"\t", key, e["value"], e["curr_state"])
                external_transition += "{0}".format(5*"\t" + "s.phase = " + e["new_state"] + ";\n")
                external_transition += "{0}".format(4*"\t" + "}\n")

            external_transition += "{0}".format("\t\t\t}")

        # for e in external_transition_per_ports:
        map["_EXTERNAL_TRANSITION_"] = external_transition
        #############################################
        output = ""
        for e in model["output"]:
            output += 2 * "\t"
            output += "if({0} == {1}) {{ {2} }}".format(
                                                        "s.phase",
                                                        map["_PHASE_NAME_"] + "::" + e["curr_state"], 
                                                        e["port"] + "->addMessage(" + e["value"] + ");"
                                                        )
            output += "\n"
        
        map["_OUTPUT_"] = output

        # print(states)
        atomics.append(replaceTokens(atomicTemplateFile.read(), map))
        # return replaceTokens(atomicTemplateFile.read(), map)
        # print()
    for a in atomics:
        print(a)


data = open("example_data.txt","r").read()

print(parseData(json.loads(data)))

# print(json.loads(f)['top'])