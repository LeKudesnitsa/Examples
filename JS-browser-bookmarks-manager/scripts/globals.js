function Tab(index, localBrowserID, URL, title){ //constructors = big letter
	this.index = index;
	this.localBrowserID = localBrowserID;
	this.URL = URL;
	this.title = title;
	// anchor = false;
}

function Session(sID, title, numberOfTabs, date, tabs){
	this.sID = sID;
	this.title = title;
	this.numberOfTabs = numberOfTabs;
	this.date = date;
	this.tabs = tabs;
	tabAnchor = 0;
	isTemp = false;
	//uniqueID = timestamp (from Date()) + device ID ///not safe btw
}

var sessionsStruct = {
	numberOfSessions : 0,
	sessions : [] //last session = last saved
	
};


var options = {
	//global
	showTabsMenu : true,
	chkWideMode : false, 

	//sessions
	chkAddDateToName : true,
	emptyNewSession : false,

	// currentSession 
	currentSession : {
		sID : 0,
		index: 0
	},
	//tabs
	chkWithCountingNumbers : true,
	chkTitleInsteadOfLink : false,
	chkAddFromLeft : false,
	chkAddFromRight : false,
	chkAddOnlyThisWindow : false,

	expWithIndexes : false,
	expWithSessionNames : false,
	expDecodePercentEncoding : false,

	imToThisSession : false,
	imStringsAreNames : false,
	imSortByTextInput : false,
	imAddHttps : false,
	importInProcess : false,

	// Waiting for Workspace API from devs...
	// onlyThisWorkspace : false,
	

	//savedOnCLose : false,
	test : 1
	
};

// var fieldState = []; //partTimeVisibleElements.groups
// var imex = "default"; //partTimeVisibleElements.layoutMode
// var alerted = false;

var partTimeVisibleElements = { 
	groups : [], //of related elemnts div (class=tabs_list and class=undeground)
	layoutMode : "default"
};

// function stateObject (type, els) {
function ptveGroupElement (type, els) {
	this.type = type;
	this.els = els; //elements
	// this.els = stateObjectElement(new )
}


//for import 
var messageHolded = false;