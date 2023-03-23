function saveOptions(){

	//TODO: SAVE ONLY OPTIONS (WHEN CHECKBOX)
	
	chrome.storage.local.set(
		{'options' : JSON.stringify(options)}
	);	
	
	chrome.storage.local.set(
		{'sessionsStruct' : JSON.stringify(sessionsStruct)}
	);
	
}

function restoreOptions(callback){
	// alert("rO");
	
	// chrome.storage.local.get(null, function(items) {
	// 	// console.log(Object.values(items));
	// 	var allKeys = Object.keys(items);
	// 	// console.log(allKeys);
	// });
	
	chrome.storage.local.get(
		'options', 
		function(data){ 

			if(!Object.keys(data).length){
				// alert("options empty!");
				return;
			}
			
			//document.querySelector(".tabs_list").style.display = "none";

			options = JSON.parse(data.options);
			console.log("lmao");
			toggleWideMode();
			
			
			// document.getElementById("param_date_in_name").checked = options.chkAddDateToName;
			// Waiting for Workspace API from devs...
			// document.getElementById("param_only_this_workspace").checked = options.onlyThisWorkspace;
			document.getElementById("param_show_tabs_menu").checked = options.showTabsMenu;
			document.getElementById("param_empty_new_session").checked = options.emptyNewSession;
			document.getElementById("param_title_instead_of_link").checked = options.chkTitleInsteadOfLink;
			document.getElementById("param_URL_list_witn_numbers").checked = options.chkWithCountingNumbers;
			document.getElementById("param_add_left").checked = options.chkAddFromLeft;
			document.getElementById("param_add_right").checked = options.chkAddFromRight;
			document.getElementById("param_add_this_window").checked = options.chkAddOnlyThisWindow;
			document.getElementById("param_wide_mode").checked = options.chkWideMode;
			
			document.getElementById("exp_with_indexes").checked = options.expWithIndexes;
			document.getElementById("exp_with_session_names").checked = options.expWithSessionNames;
			document.getElementById("exp_decode_percent_encoding").checked = options.expDecodePercentEncoding;

			document.getElementById("im_to_this_session").checked = options.imToThisSession;
			document.getElementById("im_string_is_ses_name").checked = options.imStringsAreNames;
			document.getElementById("im_add_https").checked = options.imAddHttps;
			// document.getElementById("im_sort_by_text_input").checked = options.imSortByTextInput;


			

						
			if (options.showTabsMenu){
				document.querySelector(".tabs_manage").style.display = "block";
			}

			

			//Node.rel = 'stylesheet alternate';

			
			// alert("local.get no error");
			// alert(options.test);
			// alert(options.currentSession);
			// document.getElementById("debug_menu").innerHTML = "<br>"+ options.currentSession.sID +" "+ options.currentSession.index;
			
			// alert(options.savedOnCLose);
			// options.savedOnCLose = false;
			// alert(options.savedOnCLose);
		}
	);
	
	
	//нужно обработать чекбоксы
	/*
		document.getElementById("param_URL_list_witn_numbers").checked = data.param_URL_list_witn_numbers;
	*/
	
	chrome.storage.local.get(
		'sessionsStruct', 
		function(data){ 
			if(!Object.keys(data).length){
				//alert("sessions empty!");
				return;
			}
			
			sessionsStruct = JSON.parse(data.sessionsStruct);
			//нужно отслеживать выбранную сессию; если запомнена та, которой нет, не показывать вовсе
			
			
			// updateIndexCS();
			// var str = JSON.stringify(sessionsStruct.sessions[options.currentSession.index].tabs, null, '\t');
			// console.log(str);

			console.log(sessionsStruct);



			
			listManipulatorSessions(true);
			listManipulatorTabs("update", options.currentSession.index, 0);
			callback();
		}
	);

}

/*

function saveOptions(){
	chrome.storage.local.set(
		{'param_URL_list_witn_numbers' : document.getElementById("param_URL_list_witn_numbers").checked}
	);
	
}

function restoreOptions(){
	chrome.storage.local.get(
		'param_URL_list_witn_numbers', 
		function(data){ document.getElementById("param_URL_list_witn_numbers").checked = data.param_URL_list_witn_numbers;}
	);

}
*/
