var fs = require('fs');

const envsToWrite = {};

Object.keys(process.env)
    .filter((env) => !env.startsWith('npm'))
    .forEach((env) => (envsToWrite[env] = process.env[env]));

const packageJson = require('../package.json');
envsToWrite['appVersion'] = packageJson.version;

fs.writeFile(
    'public/env-config.js',
    `/* eslint-disable prettier/prettier */ window._env_ = ${JSON.stringify(envsToWrite, null, 4)}`,
    'utf8',
    () => {
        console.log('environment variables extracted');
    }
);
