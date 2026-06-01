package main

import (
	"bufio"
	"fmt"
	"html"    // Remove "_" after TODO 1
	"os"
	"strings" // Remove "_" after TODO 1
)

// processMarkdown reads the file line-by-line and applies formatting 
func processMarkdown(scanner *bufio.Scanner, file *os.File) {

	inList := false

	LineLoop:
	for scanner.Scan() {
		// TODO 1: Extract and Sanitize
		// 1. Read the current line and remove extra spaces on the ends.
		// 2. Sanitize the text using html.EscapeString() to prevent XSS attacks
		// 3. Store this safe string in a variable called 'line'.
		text := scanner.Text()
		trimmed_text := strings.TrimSpace(text)
		line := html.EscapeString(trimmed_text)

		// fmt.Printf("Line %q\n", line)

		// TODO 2: Handle Empty Lines
		// If the line is exactly "", check if we are currently inside a list.
		// If we are, write "</ul>\n" to the file, set inList to false, and 'continue' to the next line.
		// If we aren't in a list, just 'continue'.
		
		if line == "" {
			if inList {
				fmt.Printf("In list\n")
				file.WriteString("</ul>\n")
				inList = false
			}
			continue
		}



		// TODO 3: Parse Headings H1-H6
		// Check if the line starts with "###### ", "##### ", etc.
		// IMPORTANT: Always check for H6 first, going down to H1 to avoid false matches.
		// If a match is found:
		//   1. Remove the "#" symbols.
		//   2. Pass the remaining text into your parseInline() function.
		//   3. Wrap the result in the correct HTML tags and write to the file.
		//   4. 'continue' to the next line.

		for level:=6;level >= 1;level-- {
			prefix:= strings.Repeat("#", level) + " "

			if strings.HasPrefix(line, prefix) {
				if inList {
					file.WriteString("</ul>\n")
					inList = false
				}
				content := strings.TrimPrefix(line, prefix)
				content = parseInline(content)

				fmt.Printf("Found level: %d content: %s\n", level, content)
				fmt.Fprintf(file, "<h%d>%s</h%d>\n", level, content, level)
				continue LineLoop
			}

		}




		// TODO 4: Manage List State
		// Check if the line starts with "- ".
		// If it does:
		//   1. If we are NOT already in a list, write "<ul>\n" and set inList = true.
		//   2. Trim the "- " prefix.
		//   3. Pass the content into parseInline().
		//   4. Wrap the result in <li>...</li>\n tags, write to the file, and 'continue'.
		if strings.HasPrefix(line, "- ") {
			if !inList {
				file.WriteString("<ul>\n")
				inList = true
				
			}
			content := strings.TrimPrefix(line, "- ")
			content = parseInline(content)
			fmt.Fprintf(file, "<li>%s</li>\n", content)
			continue
		}



		// TODO 5: Close the list if we hit a normal paragraph
		// If the code reaches here, it means this line is NOT empty, NOT a heading, and NOT a list item.
		// If inList is currently true, write "</ul>\n" and set inList = false.
		if inList {
			inList = false
			file.WriteString("</ul>\n")
		}

		// TODO 6: Handle standard paragraphs
		// Pass the line into parseInline(), wrap it in <p>...</p>\n tags, and write to the file.
		content := parseInline(line)
		fmt.Fprintf(file, "<p>%s</p>\n", content)

	}

	// TODO 7: Final List Cleanup
	// The file might end while we are still building a list. 
	// After the for-loop finishes, check if inList is true. If so, write "</ul>\n".
	if inList {
		inList = false
		file.WriteString("</ul>\n")

	}

	

	// TODO 8: Scan Failure Check
	// The scanner loop above stops on EOF, but also stops if the file read crashes.
	// Check if scanner.Err() != nil. If there is an error, print it to the console.
	if err := scanner.Err(); err != nil {
		fmt.Printf("Error reading: %v", err)
	}



	
}

// parseInline handles bold, italic, and inline code formatting
func parseInline(text string) string {

	// TODO 9: Parse Bold (**text**)
	// 1. Break the string into a slice of parts using strings.Split.
	// 2. Loop through the slice. Every odd index (1, 3, 5...) represents text that was inside the **.
	// 3. Wrap those odd-indexed parts in <strong>...</strong> tags.
	// 4. Put the string back together using strings.Join.
	text_split := strings.Split(text, "**")
	for i:= 1; i < len(text_split);i+=2 {
		text_split[i] = "<strong>" +text_split[i] +"</strong>"
	}
	text = strings.Join(text_split, "")


	// TODO 10: Parse Italic (*text*)
	// Repeat the exact same split-and-join process as above, but split on "*" and use <em>...</em> tags.
	text_split = strings.Split(text, "*")
	for i:= 1; i < len(text_split);i+=2 {
		text_split[i] = "<em>" +text_split[i] +"</em>"
	}
	text = strings.Join(text_split, "")


	// TODO 11: Parse Inline Code (`code`)
	// Repeat the exact same split-and-join process, but split on "`" and use <code>...</code> tags.
	text_split = strings.Split(text, "`")
	for i:= 1; i < len(text_split);i+=2 {
		text_split[i] = "<code>" +text_split[i] +"</code>"
	}
	text = strings.Join(text_split, "")

	return text
}

func main() {
	fmt.Println("=== Markdown to HTML Generator ===")

	// 1. Open the input Markdown file
	inputFile, err := os.Open("input.md")
	if err != nil {
		fmt.Printf("Error opening file: %v\n", err)
		os.Exit(1)
	}
	defer inputFile.Close()

	// 2. Create the output HTML file
	outputFile, err := os.Create("output.html")
	if err != nil {
		fmt.Printf("Error creating file: %v\n", err)
		os.Exit(1)
	}
	defer outputFile.Close()

	// 3. Coordinate the generation process
	writeHeader(outputFile)

	// Create a scanner to read the file line by line
	scanner := bufio.NewScanner(inputFile)
	processMarkdown(scanner, outputFile)

	writeFooter(outputFile)

	fmt.Println("Success! HTML generated at output.html")
}

// writeHeader handles the HTML boilerplate and CSS styling (DO NOT MODIFY)
func writeHeader(file *os.File) {
	file.WriteString("<!DOCTYPE html>\n<html>\n<head>\n")
	file.WriteString("<title>Generated Page</title>\n")
	file.WriteString("<style>\n")
	file.WriteString("  body { font-family: 'Segoe UI', Tahoma, sans-serif; max-width: 800px; margin: 40px auto; line-height: 1.6; color: #333; padding: 0 20px; }\n")
	file.WriteString("  h1 { border-bottom: 2px solid #eee; padding-bottom: 0.3em; }\n")
	file.WriteString("  h2 { border-bottom: 1px solid #eee; padding-bottom: 0.2em; }\n")
	file.WriteString("  code { background: #f4f4f4; padding: 2px 6px; border-radius: 4px; font-family: monospace; font-size: 0.9em; }\n")
	file.WriteString("  ul { padding-left: 2em; }\n")
	file.WriteString("  li { margin: 4px 0; }\n")
	file.WriteString("</style>\n")
	file.WriteString("</head>\n<body>\n")
}

// writeFooter closes the HTML tags (DO NOT MODIFY)
func writeFooter(file *os.File) {
	file.WriteString("</body>\n</html>\n")
}


