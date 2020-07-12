#!/usr/bin/env python

import json
import yaml

with open("supported_build_configurations.yaml", 'r') as stream:
    build_configs_yaml = yaml.safe_load(stream)

    build_configs_dict = {'env': []}
    for app in build_configs_yaml:
        for target in build_configs_yaml[app]:
            build_configs_dict["env"].append({"APP": app, "TARGET": target})

    build_configs_json = json.dumps(build_configs_dict)
    print(build_configs_json)
