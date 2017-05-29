// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


InspectorBackendStub = function()
{
    this._lastCallbackId = 1;
    this._pendingResponsesCount = 0;
    this._callbacks = {};
    this._domainDispatchers = {};
    this._registerDelegate('{"method": "Inspector.didEvaluateForTestInFrontend", "params": {"testCallId": {"optional": false, "type": "number"},"jsonResult": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Page.addScriptToEvaluateOnLoad", "params": {"scriptSource": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Page.removeAllScriptsToEvaluateOnLoad", "id": 0}');
    this._registerDelegate('{"method": "Page.reloadPage", "params": {"ignoreCache": {"optional": true , "type": "boolean"}}, "id": 0}');
    this._registerDelegate('{"method": "Page.openInInspectedWindow", "params": {"url": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Page.setUserAgentOverride", "params": {"userAgent": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Page.getCookies", "id": 0}');
    this._registerDelegate('{"method": "Page.deleteCookie", "params": {"cookieName": {"optional": false, "type": "string"},"domain": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Runtime.evaluate", "params": {"expression": {"optional": false, "type": "string"},"objectGroup": {"optional": false, "type": "string"},"includeCommandLineAPI": {"optional": true , "type": "boolean"}}, "id": 0}');
    this._registerDelegate('{"method": "Runtime.evaluateOn", "params": {"objectId": {"optional": false, "type": "string"},"expression": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Runtime.getProperties", "params": {"objectId": {"optional": false, "type": "string"},"ignoreHasOwnProperty": {"optional": false, "type": "boolean"}}, "id": 0}');
    this._registerDelegate('{"method": "Runtime.setPropertyValue", "params": {"objectId": {"optional": false, "type": "string"},"propertyName": {"optional": false, "type": "string"},"expression": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Runtime.releaseObject", "params": {"objectId": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Runtime.releaseObjectGroup", "params": {"objectGroup": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Console.enable", "id": 0}');
    this._registerDelegate('{"method": "Console.disable", "id": 0}');
    this._registerDelegate('{"method": "Console.clearConsoleMessages", "id": 0}');
    this._registerDelegate('{"method": "Console.setMonitoringXHREnabled", "params": {"enabled": {"optional": false, "type": "boolean"}}, "id": 0}');
    this._registerDelegate('{"method": "Console.addInspectedNode", "params": {"nodeId": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "Network.enable", "id": 0}');
    this._registerDelegate('{"method": "Network.disable", "id": 0}');
    this._registerDelegate('{"method": "Network.getCachedResources", "id": 0}');
    this._registerDelegate('{"method": "Network.getResourceContent", "params": {"frameId": {"optional": false, "type": "string"},"url": {"optional": false, "type": "string"},"base64Encode": {"optional": true , "type": "boolean"}}, "id": 0}');
    this._registerDelegate('{"method": "Network.setExtraHeaders", "params": {"headers": {"optional": false, "type": "object"}}, "id": 0}');
    this._registerDelegate('{"method": "Database.getDatabaseTableNames", "params": {"databaseId": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "Database.executeSQL", "params": {"databaseId": {"optional": false, "type": "number"},"query": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "DOMStorage.getDOMStorageEntries", "params": {"storageId": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "DOMStorage.setDOMStorageItem", "params": {"storageId": {"optional": false, "type": "number"},"key": {"optional": false, "type": "string"},"value": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "DOMStorage.removeDOMStorageItem", "params": {"storageId": {"optional": false, "type": "number"},"key": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "ApplicationCache.getApplicationCaches", "id": 0}');
    this._registerDelegate('{"method": "DOM.getDocument", "id": 0}');
    this._registerDelegate('{"method": "DOM.getChildNodes", "params": {"nodeId": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.querySelector", "params": {"nodeId": {"optional": false, "type": "number"},"selectors": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.querySelectorAll", "params": {"nodeId": {"optional": false, "type": "number"},"selectors": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.setNodeName", "params": {"nodeId": {"optional": false, "type": "number"},"name": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.setNodeValue", "params": {"nodeId": {"optional": false, "type": "number"},"value": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.removeNode", "params": {"nodeId": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.setAttribute", "params": {"elementId": {"optional": false, "type": "number"},"name": {"optional": false, "type": "string"},"value": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.removeAttribute", "params": {"elementId": {"optional": false, "type": "number"},"name": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.getEventListenersForNode", "params": {"nodeId": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.copyNode", "params": {"nodeId": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.getOuterHTML", "params": {"nodeId": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.setOuterHTML", "params": {"nodeId": {"optional": false, "type": "number"},"outerHTML": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.performSearch", "params": {"query": {"optional": false, "type": "string"},"runSynchronously": {"optional": false, "type": "boolean"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.cancelSearch", "id": 0}');
    this._registerDelegate('{"method": "DOM.pushNodeToFrontend", "params": {"objectId": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.setSearchingForNode", "params": {"enabled": {"optional": false, "type": "boolean"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.highlightDOMNode", "params": {"nodeId": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.hideDOMNodeHighlight", "id": 0}');
    this._registerDelegate('{"method": "DOM.highlightFrame", "params": {"frameId": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.hideFrameHighlight", "id": 0}');
    this._registerDelegate('{"method": "DOM.pushNodeByPathToFrontend", "params": {"path": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "DOM.resolveNode", "params": {"nodeId": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "CSS.getStylesForNode", "params": {"nodeId": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "CSS.getComputedStyleForNode", "params": {"nodeId": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "CSS.getInlineStyleForNode", "params": {"nodeId": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "CSS.getAllStyleSheets", "id": 0}');
    this._registerDelegate('{"method": "CSS.getStyleSheet", "params": {"styleSheetId": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "CSS.getStyleSheetText", "params": {"styleSheetId": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "CSS.setStyleSheetText", "params": {"styleSheetId": {"optional": false, "type": "string"},"text": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "CSS.setPropertyText", "params": {"styleId": {"optional": false, "type": "object"},"propertyIndex": {"optional": false, "type": "number"},"text": {"optional": false, "type": "string"},"overwrite": {"optional": false, "type": "boolean"}}, "id": 0}');
    this._registerDelegate('{"method": "CSS.toggleProperty", "params": {"styleId": {"optional": false, "type": "object"},"propertyIndex": {"optional": false, "type": "number"},"disable": {"optional": false, "type": "boolean"}}, "id": 0}');
    this._registerDelegate('{"method": "CSS.setRuleSelector", "params": {"ruleId": {"optional": false, "type": "object"},"selector": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "CSS.addRule", "params": {"contextNodeId": {"optional": false, "type": "number"},"selector": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "CSS.getSupportedCSSProperties", "id": 0}');
    this._registerDelegate('{"method": "Timeline.start", "id": 0}');
    this._registerDelegate('{"method": "Timeline.stop", "id": 0}');
    this._registerDelegate('{"method": "Debugger.enable", "id": 0}');
    this._registerDelegate('{"method": "Debugger.disable", "id": 0}');
    this._registerDelegate('{"method": "Debugger.setBreakpointsActive", "params": {"active": {"optional": false, "type": "boolean"}}, "id": 0}');
    this._registerDelegate('{"method": "Debugger.setBreakpointByUrl", "params": {"url": {"optional": false, "type": "string"},"lineNumber": {"optional": false, "type": "number"},"columnNumber": {"optional": true , "type": "number"},"condition": {"optional": true , "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Debugger.setBreakpoint", "params": {"location": {"optional": false, "type": "object"},"condition": {"optional": true , "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Debugger.removeBreakpoint", "params": {"breakpointId": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Debugger.continueToLocation", "params": {"location": {"optional": false, "type": "object"}}, "id": 0}');
    this._registerDelegate('{"method": "Debugger.stepOver", "id": 0}');
    this._registerDelegate('{"method": "Debugger.stepInto", "id": 0}');
    this._registerDelegate('{"method": "Debugger.stepOut", "id": 0}');
    this._registerDelegate('{"method": "Debugger.pause", "id": 0}');
    this._registerDelegate('{"method": "Debugger.resume", "id": 0}');
    this._registerDelegate('{"method": "Debugger.editScriptSource", "params": {"sourceID": {"optional": false, "type": "string"},"newContent": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Debugger.getScriptSource", "params": {"sourceID": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Debugger.setPauseOnExceptions", "params": {"state": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Debugger.evaluateOnCallFrame", "params": {"callFrameId": {"optional": false, "type": "string"},"expression": {"optional": false, "type": "string"},"objectGroup": {"optional": false, "type": "string"},"includeCommandLineAPI": {"optional": true , "type": "boolean"}}, "id": 0}');
    this._registerDelegate('{"method": "BrowserDebugger.setDOMBreakpoint", "params": {"nodeId": {"optional": false, "type": "number"},"type": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "BrowserDebugger.removeDOMBreakpoint", "params": {"nodeId": {"optional": false, "type": "number"},"type": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "BrowserDebugger.setEventListenerBreakpoint", "params": {"eventName": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "BrowserDebugger.removeEventListenerBreakpoint", "params": {"eventName": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "BrowserDebugger.setXHRBreakpoint", "params": {"url": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "BrowserDebugger.removeXHRBreakpoint", "params": {"url": {"optional": false, "type": "string"}}, "id": 0}');
    this._registerDelegate('{"method": "Profiler.enable", "id": 0}');
    this._registerDelegate('{"method": "Profiler.disable", "id": 0}');
    this._registerDelegate('{"method": "Profiler.isEnabled", "id": 0}');
    this._registerDelegate('{"method": "Profiler.start", "id": 0}');
    this._registerDelegate('{"method": "Profiler.stop", "id": 0}');
    this._registerDelegate('{"method": "Profiler.getProfileHeaders", "id": 0}');
    this._registerDelegate('{"method": "Profiler.getProfile", "params": {"type": {"optional": false, "type": "string"},"uid": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "Profiler.removeProfile", "params": {"type": {"optional": false, "type": "string"},"uid": {"optional": false, "type": "number"}}, "id": 0}');
    this._registerDelegate('{"method": "Profiler.clearProfiles", "id": 0}');
    this._registerDelegate('{"method": "Profiler.takeHeapSnapshot", "params": {"detailed": {"optional": false, "type": "boolean"}}, "id": 0}');
    this._registerDelegate('{"method": "Profiler.collectGarbage", "id": 0}');
}

InspectorBackendStub.prototype = {
    _wrap: function(callback)
    {
        var callbackId = this._lastCallbackId++;
        this._callbacks[callbackId] = callback || function() {};
        return callbackId;
    },

    _registerDelegate: function(requestString)
    {
        var domainAndFunction = JSON.parse(requestString).method.split(".");
        var agentName = domainAndFunction[0] + "Agent";
        if (!window[agentName])
            window[agentName] = {};
        window[agentName][domainAndFunction[1]] = this.sendMessageToBackend.bind(this, requestString);
    },

    sendMessageToBackend: function()
    {
        var args = Array.prototype.slice.call(arguments);
        var request = JSON.parse(args.shift());
        var callback = (args.length && typeof args[args.length - 1] === "function") ? args.pop() : 0;
        var domainAndMethod = request.method.split(".");
        var agentMethod = domainAndMethod[0] + "Agent." + domainAndMethod[1];

        if (request.params) {
            for (var key in request.params) {
                var typeName = request.params[key].type;
                var optionalFlag = request.params[key].optional;

                if (args.length === 0 && !optionalFlag) {
                    console.error("Protocol Error: Invalid number of arguments for method '" + agentMethod + "' call. It should have the next arguments '" + JSON.stringify(request.params) + "'.");
                    return;
                }

                var value = args.shift();
                if (optionalFlag && typeof value === "undefined") {
                    delete request.params[key];
                    continue;
                }

                if (typeof value !== typeName) {
                    console.error("Protocol Error: Invalid type of argument '" + key + "' for method '" + agentMethod + "' call. It should be '" + typeName + "' but it is '" + typeof value + "'.");
                    return;
                }

                request.params[key] = value;
            }
        }

        if (args.length === 1 && !callback) {
            if (typeof args[0] !== "undefined") {
                console.error("Protocol Error: Optional callback argument for method '" + agentMethod + "' call should be a function but its type is '" + typeof args[0] + "'.");
                return;
            }
        }
        request.id = this._wrap(callback || function() {});

        if (window.dumpInspectorProtocolMessages)
            console.log("frontend: " + JSON.stringify(request));

        var message = JSON.stringify(request);

        ++this._pendingResponsesCount;
        InspectorFrontendHost.sendMessageToBackend(message);
    },

    registerDomainDispatcher: function(domain, dispatcher)
    {
        this._domainDispatchers[domain] = dispatcher;
    },

    dispatch: function(message)
    {
        if (window.dumpInspectorProtocolMessages)
            console.log("backend: " + ((typeof message === "string") ? message : JSON.stringify(message)));

        var messageObject = (typeof message === "string") ? JSON.parse(message) : message;

        if ("id" in messageObject) { // just a response for some request
            if (messageObject.error && messageObject.error.code !== -32000)
                this.reportProtocolError(messageObject);

            var arguments = [];
            if (messageObject.result) {
                for (var key in messageObject.result)
                    arguments.push(messageObject.result[key]);
            }

            var callback = this._callbacks[messageObject.id];
            if (callback) {
                arguments.unshift(messageObject.error);
                callback.apply(null, arguments);
                --this._pendingResponsesCount;
                delete this._callbacks[messageObject.id];
            }

            if (this._scripts && !this._pendingResponsesCount)
                this.runAfterPendingDispatches();

            return;
        } else {
            var method = messageObject.method.split(".");
            var domainName = method[0];
            var functionName = method[1];
            if (!(domainName in this._domainDispatchers)) {
                console.error("Protocol Error: the message is for non-existing domain '" + domainName + "'");
                return;
            }
            var dispatcher = this._domainDispatchers[domainName];
            if (!(functionName in dispatcher)) {
                console.error("Protocol Error: Attempted to dispatch an unimplemented method '" + messageObject.method + "'");
                return;
            }

            var params = [];
            if (messageObject.params) {
                for (var key in messageObject.params)
                    params.push(messageObject.params[key]);
            }

            dispatcher[functionName].apply(dispatcher, params);
        }
    },

    reportProtocolError: function(messageObject)
    {
        var error = messageObject.error;
        console.error(error.message + "(" + error.code + "): request with id = " + messageObject.id + " failed.");
        for (var i = 0; i < error.data.length; ++i)
            console.error("    " + error.data[i]);
    },

    runAfterPendingDispatches: function(script)
    {
        if (!this._scripts)
            this._scripts = [];

        if (script)
            this._scripts.push(script);

        if (!this._pendingResponsesCount) {
            var scripts = this._scripts;
            this._scripts = []
            for (var id = 0; id < scripts.length; ++id)
                 scripts[id].call(this);
        }
    }
}

InspectorBackend = new InspectorBackendStub();
