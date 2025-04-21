package events

import "fmt"

type EventDispatcher interface {
	Dispatch(event interface{})
}

type ConsoleDispatcher struct{}

func (c *ConsoleDispatcher) Dispatch(event interface{}) {
	fmt.Printf("EVENT DISPATCHED: %#v\n", event)
}
