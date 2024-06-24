package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"sort"
	"strconv"
)

func calculateTotals(scanner *bufio.Scanner) (map[int]int, error) {
	elves := make(map[int]int)
	elfNum := 0
	for scanner.Scan() {
		line := scanner.Text()
		if len(line) == 0 {
			// Empty line lets go to next elf
			elfNum++
		} else {
			calories, err := strconv.Atoi(line)
			if err != nil {
				panic(err)
			}
			elf := elves[elfNum]
			if elf == 0 {
				// Doesn't exist
				elves[elfNum] = calories
			} else {
				elves[elfNum] = elves[elfNum] + calories
			}
		}

	}
	err := scanner.Err()
	if err != nil {
		log.Fatal(err)
	}
	return elves, err
}

func findMaxTotal(elves map[int]int) int {
	maxValue := 0
	for _, value := range elves {
		// key and value can be used here
		if value > maxValue {
			maxValue = value
		}
	}
	return maxValue
}

// Oh duh
func addTopThreeElvesCalories(elves map[int]int) int {
	calories := make([]int, 0, len(elves))
	for _, value := range elves {
		calories = append(calories, value)
	}
	sort.Sort(sort.Reverse(sort.IntSlice(calories)))

	topThreeTotal := 0
	for i := 0; i < 3 && i < len(calories); i++ {
		topThreeTotal += calories[i]
	}
	return topThreeTotal
}

func main() {
	f, err := os.Open("./input.txt")
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()
	scanner := bufio.NewScanner(f)
	elves, err := calculateTotals(scanner)
	fmt.Println("Maximum calories carried by an elf:", findMaxTotal(elves))
	fmt.Println("Sum of calories carried by the top three elves:", addTopThreeElvesCalories(elves))
}
