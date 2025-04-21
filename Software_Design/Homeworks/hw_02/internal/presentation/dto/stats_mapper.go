package dto

func MapGeneralStatsToResponse(
	animals, enclosures, feedings int) GeneralStatsResponse {
	return GeneralStatsResponse{
		Animals:    animals,
		Enclosures: enclosures,
		Feedings:   feedings,
	}
}

func MapStatsToResponse(
	total, healthy, seek, male, female, withEnclosure, withoutEnclosure int,
	animalIDs, healthyIDs, sickIDs, maleIDs, femaleIDs, withEnclosureIDs, withoutEnclosureIDs []string) StatsResponse {
	return StatsResponse{
		Total:               total,
		Healthy:             healthy,
		Seek:                seek,
		Male:                male,
		Female:              female,
		WithEnclosure:       withEnclosure,
		WithoutEnclosure:    withoutEnclosure,
		AnimalIDs:           animalIDs,
		HealthyIDs:          healthyIDs,
		SickIDs:             sickIDs,
		MaleIDs:             maleIDs,
		FemaleIDs:           femaleIDs,
		WithEnclosureIDs:    withEnclosureIDs,
		WithoutEnclosureIDs: withoutEnclosureIDs,
	}
}
