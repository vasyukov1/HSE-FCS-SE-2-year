package dto

type FeedingResponse struct {
	ID       string `json:"id"`
	AnimalID string `json:"animal_id"`
	Time     string `json:"time"`
	Food     string `json:"food"`
}
