package dto

type GeneralStatsResponse struct {
	Animals    int `json:"animals"`
	Enclosures int `json:"enclosures"`
	Feedings   int `json:"feedings"`
}

type StatsResponse struct {
	Total               int      `json:"total"`
	Healthy             int      `json:"count_healthy"`
	Seek                int      `json:"count_seek"`
	Male                int      `json:"count_male"`
	Female              int      `json:"count_female"`
	WithEnclosure       int      `json:"with_enclosure"`
	WithoutEnclosure    int      `json:"without_enclosure"`
	AnimalIDs           []string `json:"animal_ids"`
	HealthyIDs          []string `json:"healthy_ids"`
	SickIDs             []string `json:"sick_ids"`
	MaleIDs             []string `json:"male_ids"`
	FemaleIDs           []string `json:"female_ids"`
	WithEnclosureIDs    []string `json:"with_enclosure_ids"`
	WithoutEnclosureIDs []string `json:"without_enclosure_ids"`
}
