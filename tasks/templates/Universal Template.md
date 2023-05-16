<%*
	const type = await tp.system.suggester(
		["⚙️Software", "📱Android","🖥️Operating System", "📝️Note", "🌎Website", "✒️Font", "☑️Task", "💭Thought", "📘Book", "🔖Bookmark"], 
		["s", "a", "o", "n", "w", "f", "t", "th", "b", "bo"], false, "Type of Note"
	)
	let text = "- tags: "
	if (type == "s" || type == "a") text +="#⚙️Software"
	if (type == "o") text += "#🖥️OperatingSystem"
	if (type == "n") text += "#📝️Note/🌲️Evergreen"
	if (type == "w") text += "#🌎Website"
	if (type == "f") text += "#✒️Font"
	if (type == "t") text += "#☑️Task"
	if (type == "th") text += "#💭Thought"
	if (type == "b") text += "#📘Book"
	if (type == "bo") text += "#🔖Bookmark"

	text += `\n- title: ${tp.file.title}`

	text += `\n- wikilinks: `
	if (type == "s") text += "[[Linux]] [[Windows]] [[macOS]]"
	if (type == "a") text += "[[Android]]\n- package: "
	if (type == "w") text += "[[Website]]\n- link: \n- auth: false/true"
	if (type == "f") text += "[[Font]] [[Monospace]] [[Slab]]"
	if (type == "th") text += "[[Thought that won't let me sleep]]"
	if (type == "b") text += "[[Book]]\n- author:\n- language: [[Russian]] [[English]]"
	
	if (type != "w" && type != "f" && type != "n" && type != "th" && type != "b"  && type != "bo") text += "\n- parent:"
	
	if (type != "t" && type != "th") text += "\n- source:"
	
	if (type == "s" || type == "a") text += "\n- dependencies:"
	
	if (type != "b") text += `\n- created: ${tp.date.now("YYYY-MM-DDTHH:mm:ssZ")}`
	if (type == "b") text += `\n- started: unknown about ${tp.date.now("YYYY-MM-DDTHH:mm:ssZ")}`
	if (type != "t" && type != "b") text += `\n- updated: ${tp.date.now("YYYY-MM-DDTHH:mm:ssZ")}`
	if (type == "t") text += "\n- completed: date"
	if (type == "b") text += `\n- completed: unknown about ${tp.date.now("YYYY-MM-DDTHH:mm:ssZ")}`
	
	//let title = await tp.system.prompt("What is the name of your new note?")
	//await tp.file.rename(type + title)
	text += "\n"
	if (type != "b") text += "\n### Description\n\n\n"
	if (type == "b") {
		text += "\n### 🚀 The Book in 3 Sentences\n"
		text += "\n### 🎨 Impressions\n"
		text += "\n### 📖 Who Should Read It?\n"
		text += "\n### ☘️ How the Book Changed Me\n\nHow my life / behaviour / thoughts / ideas have changed as a result of reading the book.\n"
		text += "\n### ✍️ My Top 3 Quotes\n"
		text += "\n### 📒 Summary + Notes\n"
	}
	//text += "\n### 🔗Linkn\n\n"
_%>
`class:meta`
<%text%>