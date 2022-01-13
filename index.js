const { register, getDWord, unregister } = require("./index.node")

function registerAddon(params, cb) {
    let handle = register(params.key, params.value)
    params["handle"] = handle

// setInterval()

    return params
}

function getDWordAddon(params, defValue) {
    return getDWord(params.handle, params.value, defValue || 0)
}

function unregisterAddon(params) {
    // clearInterval
    unregister(params.handle)
}

exports.register = registerAddon
exports.getDWord = getDWordAddon
exports.unregister = unregisterAddon