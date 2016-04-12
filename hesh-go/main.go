package main

import (
	"os"
	"log"
	"time"
	"fmt"
	"bufio"
	"strings"
	"os/exec"
)

const hesh_line_format_string string = "hesh-go::%s > "
const change_directory_string = "cd"
const hesh_exit_string = "exit"

var logger *log.Logger
var currentDir string

func main() {

	var terminate error = nil
	var file *os.File
	logger, file = makeHeshLogger()
	defer endSession(logger, file)

	if err := os.Chdir(os.Getenv("HOME")); err != nil {
		logger.Fatalln(err, "could not change to $HOME directory")
	}

	for terminate == nil {
		wd, err := os.Getwd()
		if err != nil {
			logger.Fatalln(err, "could not get current working directory")
		}
		currentDir = wd

		fmt.Printf(hesh_line_format_string, wd)

		tokens := heshParseInput()

		terminate = heshExecuteTokens(tokens)
	}

}

func heshParseInput() ([]string) {
	var line string = ""
	tokens := make([]string, 0, 10)
	sca := bufio.NewScanner(os.Stdin)
	sca.Split(bufio.ScanLines)

	if sca.Scan() {
		line = sca.Text()
	}

	sca = bufio.NewScanner(strings.NewReader(line))
	sca.Split(bufio.ScanWords)

	for sca.Scan() {
		tokens = append(tokens, sca.Text())
	}

	if err := sca.Err(); err != nil {
		logger.Println(err, "could not parse line")
	}

	return tokens;
}

func heshExecuteTokens(tokens []string) error {

	pos := 0
	length := len(tokens)
	if length > 0 {

		if tokens[pos] == hesh_exit_string {
			return fmt.Errorf("exit")
		}

		if tokens[pos] == change_directory_string {
			if length > 1 {
				err := os.Chdir(tokens[pos + 1])
				logger.Println(err, tokens)
			}
		} else {
			commandChain, err := makeCommandChain(tokens)
			if err != nil {
				logger.Println(err)
				fmt.Fprintf(os.Stderr, "%v", err)
			} else {
				for i, command := range commandChain {
					if i == len (commandChain) - 1 {
						if err := command.Run(); err != nil {
							logger.Println(err, command.Args)
						}
					} else {
						if err := command.Start(); err != nil {
							logger.Println(err, command.Args)
						}
					}
				}
			}
		}
	}

	return nil
}

func makeHeshLogger() (*log.Logger, *os.File) {
	home := os.Getenv("HOME")
	logDir := home + "/.hesh/hesh-go/log"
	year, month, day := time.Now().Date()
	logFile := fmt.Sprintf("%s/hesh-go-log--%d-%02d-%02d.txt", logDir, year, month, day)
	const stars string = "**********"
	const hesh string = "*H*E*S*H*-*G*O*"
	const empty string = "     "

	if err := os.MkdirAll(logDir, os.ModePerm); err != nil {
		log.Fatal("could not create logging directory: " + logDir)
	}

	file, err := os.OpenFile(logFile, os.O_APPEND | os.O_CREATE | os.O_WRONLY, 0666)
	if err != nil {
		log.Fatal("could not open log file: " + logFile)
	}

	for i := 0; i < 8; i++ {
		file.WriteString(stars)
	}
	file.WriteString("\n")
	file.WriteString(stars)

	for i := 0; i < 4; i++ {
		file.WriteString(empty)
	}
	file.WriteString("  ")
	file.WriteString(hesh)
	file.WriteString("   ")
	for i := 0; i < 4; i++ {
		file.WriteString(empty)
	}
	file.WriteString(stars)
	file.WriteString("\n")
	for i := 0; i < 8; i++ {
		file.WriteString(stars)
	}
	file.WriteString("\n")

	logger := log.New(file, "hesh-go:", log.LstdFlags | log.Lmicroseconds)
	logger.Println("session started")

	return logger, file
}

func makeCommandChain(tokens []string) ([]*exec.Cmd, error) {
	pipes := make([]int, 0)
	commandChain := make([]*exec.Cmd, 0)

	for i, x := range tokens {
		if x == "|" {
			pipes = append(pipes, i)
		}
	}
	pipes = append(pipes, len(tokens))

	var start, end = 0, 0
	for _, i := range pipes {
		end = i
		commandChain = append(commandChain, makeCommand(tokens[start:end]))
		start = end + 1
	}

	for i, command := range commandChain {
		if i == 0 {
			command.Stdin = os.Stdin
		} else {
			outPipe, err := commandChain[i - 1].StdoutPipe()
			if err != nil {
				return nil, err
			}
			command.Stdin = outPipe
		}
		if i == len(commandChain) - 1 {
			command.Stdout = os.Stdout
		}

	}

	return commandChain, nil

}

func makeCommand(tokens []string) *exec.Cmd {
	cmd := exec.Command(tokens[0], tokens[1:]...)

	cmd.Stderr = os.Stderr

	return cmd
}

func endSession(logger *log.Logger, file *os.File) {
	logger.Println("session ended")
	file.WriteString("\n\n")
	if err := file.Close(); err != nil {
		log.Fatal("could not close logfile")
	}
}