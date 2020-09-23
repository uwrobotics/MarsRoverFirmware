#!/usr/bin/env python3
# This script maintains compatibility with Python 3.6.9 (default for Ubuntu 18.04)
import argparse
import json
import yaml


def print_supported_configs_json(args):
    with open('supported_build_configurations.yaml', 'r') as stream:
        build_configs_yaml = yaml.safe_load(stream)

        build_configs_dict = {'env': []}
        for app in build_configs_yaml:
            for target in build_configs_yaml[app]:
                build_configs_dict['env'].append({'APP': app, 'TARGET': target})

        build_configs_json = json.dumps(build_configs_dict)
        print(build_configs_json)


def count_supported_configs(args):
    num_tuples = 0
    with open('supported_build_configurations.yaml', 'r') as stream:
        build_configs_yaml = yaml.safe_load(stream)

        for app in build_configs_yaml:
            num_tuples += len(build_configs_yaml[app])

    print(num_tuples)


def print_supported_config_tuples(args):
    with open('supported_build_configurations.yaml', 'r') as stream:
        build_configs_yaml = yaml.safe_load(stream)

        build_configs_list = list(build_configs_yaml.items())

        desired_overall_index = args.index
        current_overall_index = 0
        current_app_index = 0

        while True:
            num_supported_targets_for_current_app = len(build_configs_list[current_app_index][1])
            if current_overall_index + num_supported_targets_for_current_app <= desired_overall_index:
                current_app_index += 1
                current_overall_index += num_supported_targets_for_current_app
            else:
                app = build_configs_list[current_app_index][0]

                target_index = desired_overall_index - current_overall_index
                target = build_configs_list[current_app_index][1][target_index]

                print(f'APP={app} TARGET={target}')
                return


def verify_config(args):
    app = args.APP
    target = args.TARGET

    print(f'Verifying config with APP={app} and TARGET={target}')

    with open('supported_build_configurations.yaml', 'r') as stream:
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
    subparsers = parser.add_subparsers(dest='subcommand', description='actions this script can perform')
    subparsers.required = True

    print_ci_json_parser = subparsers.add_parser('print-ci-json',
                                                 help='prints supported_build_configurations.yaml in json format. '
                                                      'useful for CI.')
    print_ci_json_parser.set_defaults(func=print_supported_configs_json)

    count_supported_configs_parser = subparsers.add_parser('count-supported-configs',
                                                           help='prints the number of supported build configuation '
                                                                'tuples')
    count_supported_configs_parser.set_defaults(func=count_supported_configs)

    print_supported_config_parser = subparsers.add_parser('print-supported-config',
                                                          help='prints the supported build configurations at '
                                                               'specified index')
    print_supported_config_parser.add_argument('index', type=int, action='store',
                                               help='index of supported build configuration to print')
    print_supported_config_parser.set_defaults(func=print_supported_config_tuples)

    verify_config_parser = subparsers.add_parser('verify-config',
                                                 help='verifies if build config is supported')
    verify_config_parser.add_argument('--APP', action='store', required=True, help='Name of app to verify')
    verify_config_parser.add_argument('--TARGET', action='store', required=True, help='Name of board target to verify')
    verify_config_parser.set_defaults(func=verify_config)

    args = parser.parse_args()
    args.func(args)


if __name__ == '__main__':
    main()
