package animal

import "errors"

//
// Gender VO
//

type Gender string

const (
	Male   Gender = "male"
	Female Gender = "female"
)

func NewGender(value string) (Gender, error) {
	switch value {
	case string(Male), string(Female):
		return Gender(value), nil
	default:
		return "", errors.New("invalid gender value")
	}
}

func (g Gender) IsMale() bool {
	return g == Male
}

//
// HealthStatus VO
//

type HealthStatus string

const (
	Healthy HealthStatus = "healthy"
	Sick    HealthStatus = "sick"
)

func NewHealthStatus(value string) (HealthStatus, error) {
	switch value {
	case string(Healthy), string(Sick):
		return HealthStatus(value), nil
	default:
		return "", errors.New("invalid health status value")
	}
}

func (hs HealthStatus) IsHealthy() bool {
	return hs == Healthy
}
