#!/usr/bin/env python3.8
import argparse
import json
import yaml


def print_supported_configs_json(args):
    with open("supported_build_configurations.yaml", 'r') as stream:
        build_configs_yaml = yaml.safe_load(stream)

        build_configs_dict = {'env': []}
        for app in build_configs_yaml:
            for target in build_configs_yaml[app]:
                build_configs_dict["env"].append({"APP": app, "TARGET": target})

        build_configs_json = json.dumps(build_configs_dict)
        print(build_configs_json)


def verify_config(args):
    app = args.APP
    target = args.TARGET

    print(f'Verifying config with APP={app} and TARGET={target}')

    with open("supported_build_configurations.yaml", 'r') as stream:
        build_configs_yaml = yaml.safe_load(stream)
        valid_targets = build_configs_yaml.get(app)
        if valid_targets is None:
            print(f'{app} is not a supported app for any target.')
            exit(-1)

        if target not in valid_targets:
            print(f'{target} is not a supported target for {app}. Supported targets for {app} are: ')
            for valid_target in valid_targets:
                print(valid_target)
            exit(-1)

        print(f'{app} is supported on {target}!')


def main():
    parser = argparse.ArgumentParser(description='A collection of helpers related to UWRT Firmware build configs')
    subparsers = parser.add_subparsers(required=True, dest='subcommand', description='actions this script can perform')

    verify_config_parser = subparsers.add_parser('print_ci_json',
                                                 help='prints supported_build_configurations.yaml in json format. useful for CI.')
    verify_config_parser.set_defaults(func=print_supported_configs_json)

    verify_config_parser = subparsers.add_parser('verify_config',
                                                 help='verifies if build config is supported')
    verify_config_parser.add_argument('--APP', action='store', required=True, help='Name of app to verify')
    verify_config_parser.add_argument('--TARGET', action='store', required=True, help='Name of board target to verify')
    verify_config_parser.set_defaults(func=verify_config)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
