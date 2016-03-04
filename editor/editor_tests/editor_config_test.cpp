#include "testing/testing.hpp"

#include "editor/editor_config.hpp"

#include "std/set.hpp"

using namespace editor;

UNIT_TEST(EditorConfig_TypeDescription)
{
  using EType = feature::Metadata::EType;
  using TFields = editor::TypeAggregatedDescription::TFeatureFields;

  TFields const poi = {
    feature::Metadata::FMD_OPEN_HOURS,
    feature::Metadata::FMD_PHONE_NUMBER,
    feature::Metadata::FMD_WEBSITE,
    feature::Metadata::FMD_EMAIL
  };

  EditorConfig config;

  {
    editor::TypeAggregatedDescription desc;
    TEST(!config.GetTypeDescription({"death-star"}, desc), ());
  }
  {
    editor::TypeAggregatedDescription desc;
    TEST(config.GetTypeDescription({"amenity-hunting_stand"}, desc), ());
    TEST(desc.IsNameEditable(), ());
    TEST(!desc.IsAddressEditable(), ());
    TEST_EQUAL(desc.GetEditableFields(), TFields {EType::FMD_HEIGHT}, ());
  }
  {
    editor::TypeAggregatedDescription desc;
    TEST(config.GetTypeDescription({"shop-toys"}, desc), ());
    TEST(desc.IsNameEditable(), ());
    TEST(desc.IsAddressEditable(), ());
    auto fields = poi;
    fields.push_back(EType::FMD_INTERNET);
    TEST_EQUAL(desc.GetEditableFields(), fields, ());
  }
  {
    // Select amenity-bank because it goes first in config.
    editor::TypeAggregatedDescription desc;
    TEST(config.GetTypeDescription({"amenity-bar", "amenity-bank"}, desc), ());
    TEST(desc.IsNameEditable(), ());
    TEST(desc.IsAddressEditable(), ());
    auto fields = poi;
    fields.push_back(EType::FMD_OPERATOR);
    TEST_EQUAL(desc.GetEditableFields(), fields, ());
  }
  // TODO(mgsergio): Test case with priority="high" when there is one on editor.config.
}

UNIT_TEST(EditorConfig_GetTypesThatGenBeAdded)
{
  EditorConfig config;

  auto const types = config.GetTypesThatCanBeAdded();
  TEST(find(begin(types), end(types), "amenity-cafe") != end(types), ());
  TEST(find(begin(types), end(types), "natural-peak") == end(types), ());
}
