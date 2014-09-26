#pragma once

#include "../base/base.hpp"

#include "../std/vector.hpp"


namespace feature { class TypesHolder; }
class FeatureType;

namespace ftypes
{

class BaseChecker
{
  bool IsMatched(uint32_t type) const;

protected:
  vector<uint32_t> m_types;

public:
  bool operator() (feature::TypesHolder const & types) const;
  bool operator() (FeatureType const & ft) const;
  bool operator() (vector<uint32_t> const & types) const;

  static uint32_t PrepareToMatch(uint32_t type);
};

class IsStreetChecker : public BaseChecker
{
public:
  IsStreetChecker();

  static IsStreetChecker const & Instance();
};

class IsBuildingChecker : public BaseChecker
{
public:
  IsBuildingChecker();

  uint32_t GetMainType() const { return m_types[0]; }
};

/// Type of locality (do not change values and order - they have detalization order)
/// COUNTRY < STATE < CITY < ...
enum Type { NONE = -1, COUNTRY = 0, STATE, CITY, TOWN, VILLAGE, LOCALITY_COUNT };

class IsLocalityChecker : public BaseChecker
{
public:
  IsLocalityChecker();

  Type GetType(feature::TypesHolder const & types) const;
  Type GetType(FeatureType const & f) const;

  static IsLocalityChecker const & Instance();
};

/// @name Get city radius and population.
/// @param r Radius in meters.
//@{
uint32_t GetPopulation(FeatureType const & ft);
double GetRadiusByPopulation(uint32_t p);
uint32_t GetPopulationByRadius(double r);
//@}
}
