var addon = require('bindings')('usbrelay-ui021.node')

module.exports.setRelay = addon.setRelay;
module.exports.getRelay = addon.getRelay;
module.exports.toggleRelay = addon.toggleRelay;
