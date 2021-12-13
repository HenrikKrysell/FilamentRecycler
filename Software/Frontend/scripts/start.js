const path = require('path');
const rewire = require('rewire');
const defaults = rewire('react-scripts/scripts/start.js');
const config = defaults.__get__('configFactory')('development');

const rootDirectory = path.resolve(__dirname, '../../../');
const rootNodeDirectory = path.resolve(rootDirectory, 'node_modules');

config.resolve.alias = {
    'core-js/modules': path.resolve(rootNodeDirectory, 'core-js/modules'),
    'react-native$': path.resolve(rootNodeDirectory, 'react-native-web'),
    'react-native-svg': path.resolve(rootNodeDirectory, 'react-native-svg-web'),
};

config.module.rules[1].oneOf.forEach((rule) => {
    if (rule && rule.options) {
        if (rule.options.configFile !== undefined) {
            rule.options.configFile = true;
        }
    }
});

defaults.__set__('configFactory', () => {
    return config;
});
