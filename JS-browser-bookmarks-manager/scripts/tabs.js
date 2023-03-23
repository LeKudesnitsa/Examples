function addThisTab(){
	updateIndexCS();
	chrome.tabs.query({active: true, currentWindow: true}, function(tabs) {
		var currTab = tabs[0];
		if (currTab) { 

			sessionsStruct.sessions[options.currentSession.index].tabs.push(new Tab(
				sessionsStruct.sessions[options.currentSession.index].tabs.length,
				currTab.index, currTab.url, currTab.title));
			
			listManipulatorSessions(true);	
			listManipulatorTabs("update", options.currentSession.index, 0);

			alertResponse("green", "Current tab added!");
			divResize();
		}
	  });
}

function chromeTabsCall(func){
	var tabs =  [];
	chrome.tabs.query({}, 
		function(results) {
			results.forEach(function(tab) {
				tabs[tab.index] = tab;		
		});
		console.log(tabs);
		func(true, tabs);
	});
}


function addSeveralTabs(selfRunned, tabs){


	if(!selfRunned)
		chromeTabsCall(addSeveralTabs);
	else{

		updateIndexCS();
		var count = parseInt((document.getElementById("manage_tabs_count").value), 10);
		if(Number.isNaN(count)){
			alert("Enter number of tabs to add/load at first!");
			return;
		}

	}

}

function getAllTabs(selfRunned, tabs){

	if(!selfRunned)
		chromeTabsCall(getAllTabs);
	else{
		var i = 0, indexCounter = -1, prev = 0;
		var sesIdx = sessionsStruct.sessions[options.currentSession.index].tabs.length;
		
		//sorting as tabs presented in browser
		while(true){
			
			
			if (!tabs[i]){
				if(tabs[prev].index == indexCounter){
					break;
				}

				i = 0;

				//окончание по заполнению сделай
			}
			indexCounter++;
			// console.log(`i ${i} prev ${prev} idx ${tabs[i].index} iC ${indexCounter} `);

			if(tabs[i].index == indexCounter){
				// console.log("match");
				sessionsStruct.sessions[options.currentSession.index].tabs.push(new Tab(sesIdx + indexCounter, tabs[i].index, tabs[i].url, tabs[i].title));
			}
			prev = i;
			i++;
		}

		listManipulatorTabs("getAllTabs", options.currentSession.index, 0);
		listManipulatorSessions(true);
		document.querySelector(".tabs_manage").style.display = "block";

		// document.getElementById("response").innerHTML += '<font color="green"> All opened tabs added!</font>';
		alertResponse("green", "All opened tabs added!");
		divResize();
		// document.getElementById("response").display = "inline";

		tabs.splice(0, tabs.length);

		console.log(sessionsStruct);
	}
}

function tabRemove(tabID){
	var lim = sessionsStruct.sessions[options.currentSession.index].tabs.length;

	for(var i = tabID; i < lim; i++)
		sessionsStruct.sessions[options.currentSession.index].tabs[i].index--;	
		
	sessionsStruct.sessions[options.currentSession.index].tabs.splice(tabID, 1);
	
	// saveOptions();
	listManipulatorSessions(true);
	listManipulatorTabs("tabRemove", options.currentSession.index, 0);
}
