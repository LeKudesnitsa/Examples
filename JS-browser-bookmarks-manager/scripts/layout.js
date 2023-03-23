function divResize(){


	var others = 
	document.querySelector(".sessions").offsetHeight 
	+ document.querySelector(".underground").offsetHeight 
	+ document.querySelector(".tabs_manage").offsetHeight;
	// - listSeparatedHeight; 


	var fields = document.querySelector(".tabs_list").childNodes;
	var maxHeight = 0;

	if(options.chkWideMode){
		//document.body.style.minHeight = others + 13 + "px";


		// max popup height = 600px, scrollbar shows if more than ~580, 555 somehow in this case
		maxHeight = 550;



		var tlh =  document.querySelector(".tabs_list").offsetHeight;
		if (tlh >= others ){
			 document.querySelector(".root").style.height = document.body.style.height = tlh + "px";
		}else{
			document.querySelector(".root").style.height = document.body.style.height = null;
		}
		


	}else{

		document.querySelector(".root").style.height = document.body.style.height = null;

		// max popup height = 600px, scrollbar shows if more than ~580, 570 in my case
		maxHeight = (570 - others);

		document.getElementById("list_separated").style.maxHeight = maxHeight + "px";


	}

	var maxWidth = 0;

	fields.forEach(
		function(currentValue){
			if(currentValue.nodeType == 1){
				
				if(currentValue.nodeName == "TEXTAREA"){

					if(options.chkWideMode){
						maxWidth = 400;	
					}else{
						maxWidth = 345; 
						maxHeight -= 6;
					}
					currentValue.style.maxWidth = maxWidth + "px";
				}
				currentValue.style.maxHeight = maxHeight + "px";
				// currentValue.style.width = maxWidth + "px";
			}
		}
	);
}

function toggleWideMode(){


	if(options.showTabsMenu && options.chkWideMode){
		document.getElementById("wide_mode_css").rel = 'stylesheet';
		document.getElementById("narrow_mode_css").rel = 'stylesheet alternate';
	}else{
		document.getElementById("wide_mode_css").rel = 'stylesheet alternate';
		document.getElementById("narrow_mode_css").rel = 'stylesheet';		
	}


	divResize();

	
	// console.log("toggle");
	
}

function showField (mode){

	console.log("showField")

	function hideOther (needed){
		for(var i = 0, lim = partTimeVisibleElements.groups.length; i < lim; i++){
			if(partTimeVisibleElements.groups[i].type != needed){
				for(var j = 0, jlim = partTimeVisibleElements.groups[i].els.length; j < jlim; j++){
					partTimeVisibleElements.groups[i].els[j].style.display = "none";
				}
			}
		}
	}

	switch (mode){
		case "default":
			hideOther(mode);
			partTimeVisibleElements.layoutMode = mode;
			document.getElementById("list_separated").style.display = "block";
			break;

		case "export":		
			if (partTimeVisibleElements.layoutMode == mode){
				showField("default");
				return;
			}
			if(partTimeVisibleElements.layoutMode != "default"){
				showField("default");
			}
			partTimeVisibleElements.layoutMode = mode;

			//этот if полное говно, переделай
			if(document.getElementById("list_output").innerHTML != "Waiting..."){
				hideOther(mode);
				document.getElementById("list_output").style.display = "block";
				document.getElementById("copy_to_clipboard").style.display = "block";
			}
			document.getElementById("export").style.display = "block";
			document.getElementById("exp_text_options").style.display = "block";
			break;

		case "import":
			if (partTimeVisibleElements.layoutMode == mode){
				showField("default");
				console.log("==================case import if ")
				return;
			}
			console.log("================case import aif ")
			partTimeVisibleElements.layoutMode = mode;
			hideOther(mode);
			document.getElementById("import").style.display = "block";
			document.getElementById("list_input").style.display = "block";
			
			break;
	}
}


// disableButtons("dialog", true);
// disableButtons("dialog", false);
function disableButtons(group, status){
	//making grey buttons that should not be pressed
	//status: true -- disable
	//i.e. when editing imported session in preview i showing regular menu,
	//but since it's import phase, i should disable almost all of the buttons  

	//sets of element's id's
	var groups = { // all except "param_URL_list_witn_numbers", "param_title_instead_of_link"
		"all": ["session_select",
				"sessions_new",
				"sessions_delete_selected",
				"sessions_rename",
				"sessions_manage",
				"open_in_tab",
				"param_wide_mode",
				"param_show_tabs_menu",
				"param_empty_new_session",
				"session_name_field",
				"sessions_rename_ok",
				"sessions_clear",
				"options_clear",
				"detect",
				"manage_tabs_count",
				"param_add_left",
				"param_add_right",
				"param_add_this_window",
				"param_URL_list_witn_numbers",
				"param_ignore_existing",
				"param_title_instead_of_link",
				"add_tabs",
				"add_curr_tab",
				"get_all_tabs",
				"delete_all_tabs",
				"list_output",
				"list_input",
				"import_sel",
				"export_sel",
				"im_to_this_session",
				"im_string_is_ses_name",
				"im_add_https",
				"im_from_input",
				"im_from_file",
				"text_list",
				"exp_only_this",
				"exp_on_screen",
				"JSON",
				"exp_all",
				"exp_to_file",
				"exp_with_indexes",
				"exp_with_session_names",
				"exp_decode_percent_encoding",
				"copy_to_clipboard",
		],
		"dialogImportJSON": [	"session_select",
								"sessions_new",
								"sessions_delete_selected",
								"sessions_rename",
								"sessions_manage",
								"open_in_tab",
								"param_show_tabs_menu",
								"param_empty_new_session",
								"session_name_field",
								"sessions_rename_ok",
								"sessions_clear",
								"options_clear",
								"detect",
								"manage_tabs_count",
								"param_add_left",
								"param_add_right",
								"param_add_this_window",
								"param_ignore_existing",
								"add_tabs",
								"add_curr_tab",
								"get_all_tabs",
								"delete_all_tabs",
								"list_output",
								"list_input",
								"import_sel",
								"export_sel",
								"im_to_this_session",
								"im_string_is_ses_name",
								"im_add_https",
								"im_from_input",
								"im_from_file",
								"text_list",
								"exp_only_this",
								"exp_on_screen",
								"JSON",
								"exp_all",
								"exp_to_file",
								"exp_with_indexes",
								"exp_with_session_names",
								"exp_decode_percent_encoding",
								"copy_to_clipboard"
		],
		"other" : []
	}


	Object.keys(groups).forEach(function(name){
		console.log(name);
		if (name == group){
			groups[name].forEach(function(id){
				document.getElementById(id).disabled = status;
			})
		}
	})
	

}