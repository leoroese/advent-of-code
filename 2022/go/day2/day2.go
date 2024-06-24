package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"
)

func openFile(filename string) (*os.File, error) {
	f, err := os.Open(filename)
	if err != nil {
		log.Fatal(err)
	}
	return f, err
}

// For oppopnent

// For me
/*
1: rock (x)
2: paper (y)
3: scizzors (z)

loss: 0
draw: 3
win: 6
*/

var PlayerPointPlay = map[string]int{
	"X": 1,
	"Y": 2,
	"Z": 3,
}

// x is lose
// y is draw
// z is win

func playGame(line string) int {
	split := strings.Split(line, " ")
	oppPlay := split[0]
	desiredResult := split[1]

	if desiredResult == "X" {
		// need to lose
		if oppPlay == "A" {
			return PlayerPointPlay["Z"]
		} else if oppPlay == "B" {
			return PlayerPointPlay["X"]
		} else {
			return PlayerPointPlay["Y"]
		}
	}

	// need to draw
	if desiredResult == "Y" {
		if oppPlay == "A" {
			return 3 + PlayerPointPlay["X"]
		} else if oppPlay == "B" {
			return 3 + PlayerPointPlay["Y"]
		} else {
			return 3 + PlayerPointPlay["Z"]
		}
	}

	// win
	if desiredResult == "X" {
		// need to lose
		if oppPlay == "A" {
			return PlayerPointPlay["Z"]
		} else if oppPlay == "B" {
			return PlayerPointPlay["X"]
		} else {
			return PlayerPointPlay["Y"]
		}
	}

	if oppPlay == "A" {
		return 6 + PlayerPointPlay["Y"]
	} else if oppPlay == "B" {
		return 6 + PlayerPointPlay["Z"]
	}
	return 6 + PlayerPointPlay["X"]
}

func main() {
	f, _ := openFile("./input.txt")
	defer f.Close()

	scanner := bufio.NewScanner(f)
	totalPoints := 0
	for scanner.Scan() {
		line := scanner.Text()
		totalPoints += playGame(line)
	}
	fmt.Println(totalPoints)
}
