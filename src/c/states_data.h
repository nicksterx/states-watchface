#pragma once

// ---------------------------------------------------------------------------
// States Watchface — Data
//
// 60 entries, one per minute (00–59).
// :01–:50  = 50 states in order of statehood
// :51–:59  = US territories + DC
// :00      = America fact
//
// Facts rotate daily (day-of-year % fact_count) so the watch
// shows something different each day without repetition within a day.
//
// Fact strings: keep under 58 chars (fits two lines of GOTHIC_14 at 144px).
// Title:        keep under 20 chars (GOTHIC_18_BOLD).
// Subtitle:     keep under 26 chars (GOTHIC_14, muted).
// ---------------------------------------------------------------------------

#define MAX_FACTS 6

typedef struct {
  const char *title;
  const char *subtitle;
  uint8_t     fact_count;
  const char *facts[MAX_FACTS];
} MinuteEntry;

static const MinuteEntry MINUTE_DATA[60] = {

  // :00 — America
  [0] = {
    "America",
    "Est. 1776",
    6,
    {
      "3rd largest country by area & population",
      "50 states, 1 federal district, 5 territories",
      "Has the world's oldest written constitution",
      "More public libraries than McDonald's",
      "Only country to use the bald eagle as its symbol",
      "Americans eat 50 billion burgers per year"
    }
  },

  // :01 — Delaware (1st, Dec 7 1787)
  [1] = {
    "Delaware",
    "Dover \xe2\x80\xa2 Est. 1787",
    5,
    {
      "First state to ratify the Constitution",
      "Nicknamed 'The First State'",
      "More corporations HQ'd here than people",
      "No sales tax — shoppers drive from nearby states",
      "Thomas Jefferson called it 'the Diamond State'"
    }
  },

  // :02 — Pennsylvania (2nd, Dec 12 1787)
  [2] = {
    "Pennsylvania",
    "Harrisburg \xe2\x80\xa2 Est. 1787",
    5,
    {
      "Declaration of Independence signed in Philadelphia",
      "Home of the Liberty Bell",
      "Hershey, PA makes more chocolate than anywhere in the US",
      "Pittsburgh has more bridges than any city in the world",
      "Punxsutawney Phil predicts winter from here every Feb 2"
    }
  },

  // :03 — New Jersey (3rd, Dec 18 1787)
  [3] = {
    "New Jersey",
    "Trenton \xe2\x80\xa2 Est. 1787",
    5,
    {
      "Most densely populated state in the US",
      "Thomas Edison invented the lightbulb here",
      "Home to more diners than any other state",
      "First professional baseball game played here in 1846",
      "New Jersey has no self-serve gas stations (law)"
    }
  },

  // :04 — Georgia (4th, Jan 2 1788)
  [4] = {
    "Georgia",
    "Atlanta \xe2\x80\xa2 Est. 1788",
    5,
    {
      "Produces 50% of the nation's pecans",
      "Coca-Cola was invented in Atlanta in 1886",
      "Home of the world's busiest airport (Hartsfield-Jackson)",
      "Stone Mountain is the largest exposed granite in the world",
      "Martin Luther King Jr. was born in Atlanta"
    }
  },

  // :05 — Connecticut (5th, Jan 9 1788)
  [5] = {
    "Connecticut",
    "Hartford \xe2\x80\xa2 Est. 1788",
    5,
    {
      "First American cookbook published here in 1796",
      "Lollipop machine was invented here in 1908",
      "Home to America's oldest newspaper (Hartford Courant, 1764)",
      "Frisbee was invented in Bridgeport, Connecticut",
      "Has the highest per-capita income of any US state"
    }
  },

  // :06 — Massachusetts (6th, Feb 6 1788)
  [6] = {
    "Massachusetts",
    "Boston \xe2\x80\xa2 Est. 1788",
    5,
    {
      "First public school in the US opened here in 1635",
      "Basketball was invented in Springfield in 1891",
      "Home to Harvard, the oldest US university (1636)",
      "Boston Marathon is the world's oldest annual marathon",
      "The first subway in the US opened in Boston in 1897"
    }
  },

  // :07 — Maryland (7th, Apr 28 1788)
  [7] = {
    "Maryland",
    "Annapolis \xe2\x80\xa2 Est. 1788",
    5,
    {
      "Produces nearly a third of the nation's blue crabs",
      "The US Naval Academy is located in Annapolis",
      "First railroad in the US ran from Baltimore",
      "Francis Scott Key wrote the Star-Spangled Banner here",
      "Jousting is the official state sport"
    }
  },

  // :08 — South Carolina (8th, May 23 1788)
  [8] = {
    "South Carolina",
    "Columbia \xe2\x80\xa2 Est. 1788",
    5,
    {
      "First tea farm in the US was established here",
      "Home of the first public library in the US (1698)",
      "Fort Sumter, where the Civil War began, is here",
      "Produces more peaches than Georgia",
      "Spoleto Festival draws artists from around the world"
    }
  },

  // :09 — New Hampshire (9th, Jun 21 1788)
  [9] = {
    "New Hampshire",
    "Concord \xe2\x80\xa2 Est. 1788",
    5,
    {
      "First in the nation presidential primary state",
      "State motto: 'Live Free or Die'",
      "Mt. Washington has the world's worst recorded wind (231 mph)",
      "No income tax and no sales tax",
      "Alan Shepard, first American in space, was born here"
    }
  },

  // :10 — Virginia (10th, Jun 25 1788)
  [10] = {
    "Virginia",
    "Richmond \xe2\x80\xa2 Est. 1788",
    5,
    {
      "8 US presidents were born in Virginia — the most of any state",
      "The Pentagon, the world's largest office building, is here",
      "Virginia was the first permanent English settlement (1607)",
      "Arlington National Cemetery is in Virginia",
      "Home to the world's largest naval station (Norfolk)"
    }
  },

  // :11 — New York (11th, Jul 26 1788)
  [11] = {
    "New York",
    "Albany \xe2\x80\xa2 Est. 1788",
    5,
    {
      "NYC was the first US capital city",
      "The Statue of Liberty was a gift from France in 1886",
      "New York has more people than 39 states combined",
      "Toilet paper was invented in NYC in 1857",
      "The Empire State Building has its own zip code"
    }
  },

  // :12 — North Carolina (12th, Nov 21 1789)
  [12] = {
    "North Carolina",
    "Raleigh \xe2\x80\xa2 Est. 1789",
    5,
    {
      "Wright Brothers made first powered flight here in 1903",
      "Pepsi-Cola was invented in New Bern in 1893",
      "Home to the largest natural open-face gold mine in the US",
      "Virginia Dare, first English child born in the Americas, was born here",
      "Research Triangle Park is one of the largest research parks in the world"
    }
  },

  // :13 — Rhode Island (13th, May 29 1790)
  [13] = {
    "Rhode Island",
    "Providence \xe2\x80\xa2 Est. 1790",
    5,
    {
      "Smallest state — fits inside Alaska 425 times",
      "First state to declare independence from Britain (1776)",
      "First Baptist church in America founded here in 1638",
      "Doughnut was invented in Providence in 1847",
      "Home to more Gilded Age mansions than any other US city"
    }
  },

  // :14 — Vermont (14th, Mar 4 1791)
  [14] = {
    "Vermont",
    "Montpelier \xe2\x80\xa2 Est. 1791",
    5,
    {
      "Produces more maple syrup than any other US state",
      "Smallest state capital by population (Montpelier)",
      "First state to abolish slavery in its constitution (1777)",
      "Ben & Jerry's Ice Cream was founded in Burlington",
      "Vermont has more cows than people"
    }
  },

  // :15 — Kentucky (15th, Jun 1 1792)
  [15] = {
    "Kentucky",
    "Frankfort \xe2\x80\xa2 Est. 1792",
    5,
    {
      "Home of the Kentucky Derby, the oldest US horse race",
      "95% of the world's bourbon whiskey is produced here",
      "Mammoth Cave is the world's longest known cave system",
      "Abraham Lincoln was born in a log cabin in Kentucky",
      "Louisville Slugger baseball bats have been made here since 1884"
    }
  },

  // :16 — Tennessee (16th, Jun 1 1796)
  [16] = {
    "Tennessee",
    "Nashville \xe2\x80\xa2 Est. 1796",
    5,
    {
      "Nashville is called the 'Music City'",
      "Elvis Presley's Graceland is in Memphis",
      "Tennessee borders more states than any other (8)",
      "Jack Daniel's Distillery is in a dry county",
      "Home to Dollywood, Dolly Parton's theme park"
    }
  },

  // :17 — Ohio (17th, Mar 1 1803)
  [17] = {
    "Ohio",
    "Columbus \xe2\x80\xa2 Est. 1803",
    5,
    {
      "More astronauts have come from Ohio than any other state",
      "Neil Armstrong, first man on the moon, was from Ohio",
      "Cleveland hosted the first electric traffic lights in 1914",
      "Seven US presidents were born in Ohio",
      "The first ambulance service in the US started in Cincinnati"
    }
  },

  // :18 — Louisiana (18th, Apr 30 1812)
  [18] = {
    "Louisiana",
    "Baton Rouge \xe2\x80\xa2 Est. 1812",
    5,
    {
      "New Orleans is the birthplace of jazz music",
      "Louisiana is the only state with parishes instead of counties",
      "The Louisiana Purchase doubled the size of the US in 1803",
      "Mardi Gras has been celebrated in New Orleans since 1699",
      "Louisiana has more miles of waterway than any other state"
    }
  },

  // :19 — Indiana (19th, Dec 11 1816)
  [19] = {
    "Indiana",
    "Indianapolis \xe2\x80\xa2 Est. 1816",
    5,
    {
      "Indianapolis 500 is the world's largest single-day sporting event",
      "Santa Claus, Indiana gets thousands of Christmas letters yearly",
      "Popcorn was popularized at the 1893 World's Fair from Indiana",
      "More miles of highway than any other state except Texas",
      "First professional baseball game for money played here in 1871"
    }
  },

  // :20 — Mississippi (20th, Dec 10 1817)
  [20] = {
    "Mississippi",
    "Jackson \xe2\x80\xa2 Est. 1817",
    5,
    {
      "The Mississippi River is named after the state",
      "Birthplace of blues music in the Delta region",
      "Jim Henson, creator of the Muppets, was born here",
      "The teddy bear was named after Teddy Roosevelt during a Mississippi hunt",
      "Last state to ratify the 13th Amendment (officially in 2013)"
    }
  },

  // :21 — Illinois (21st, Dec 3 1818)
  [21] = {
    "Illinois",
    "Springfield \xe2\x80\xa2 Est. 1818",
    5,
    {
      "Abraham Lincoln lived in Springfield before becoming president",
      "Chicago is the birthplace of the skyscraper (1885)",
      "The Chicago River is dyed green every St. Patrick's Day",
      "Cracker Jack was introduced at the 1893 World's Fair in Chicago",
      "Deep-dish pizza was invented in Chicago in 1943"
    }
  },

  // :22 — Alabama (22nd, Dec 14 1819)
  [22] = {
    "Alabama",
    "Montgomery \xe2\x80\xa2 Est. 1819",
    5,
    {
      "Rosa Parks refused to give up her bus seat here in 1955",
      "US Space & Rocket Center is in Huntsville",
      "First electric streetcar system in the US was in Montgomery",
      "Helen Keller was born in Tuscumbia, Alabama",
      "Talladega Superspeedway is the fastest NASCAR track in the world"
    }
  },

  // :23 — Maine (23rd, Mar 15 1820)
  [23] = {
    "Maine",
    "Augusta \xe2\x80\xa2 Est. 1820",
    5,
    {
      "Produces 90% of the nation's lobster supply",
      "Easternmost point in the continental US",
      "The only state with a one-syllable name",
      "Stephen King was born and still lives in Maine",
      "More than 60% of Maine is covered by forest"
    }
  },

  // :24 — Missouri (24th, Aug 10 1821)
  [24] = {
    "Missouri",
    "Jefferson City \xe2\x80\xa2 Est. 1821",
    5,
    {
      "Gateway Arch in St. Louis is the tallest monument in the US",
      "The ice cream cone was invented at the 1904 St. Louis World's Fair",
      "Jesse James and Mark Twain were both born in Missouri",
      "Kansas City has more fountains than any city except Rome",
      "The first kindergarten in the US opened in St. Louis in 1873"
    }
  },

  // :25 — Arkansas (25th, Jun 15 1836)
  [25] = {
    "Arkansas",
    "Little Rock \xe2\x80\xa2 Est. 1836",
    5,
    {
      "Only state where diamonds are mined commercially",
      "Walmart was founded in Rogers, Arkansas in 1962",
      "Bill Clinton was born in Hope, Arkansas",
      "Arkansas produces more broiler chickens than any other state",
      "The state has 600,000 acres of lakes — more than any landlocked state"
    }
  },

  // :26 — Michigan (26th, Jan 26 1837)
  [26] = {
    "Michigan",
    "Lansing \xe2\x80\xa2 Est. 1837",
    5,
    {
      "Motor City Detroit revolutionized the auto industry",
      "Michigan has more freshwater coastline than any other state",
      "Kellogg's cereal company was founded in Battle Creek",
      "Motown Records was founded in Detroit in 1959",
      "Michigan is the only state made up of two peninsulas"
    }
  },

  // :27 — Florida (27th, Mar 3 1845)
  [27] = {
    "Florida",
    "Tallahassee \xe2\x80\xa2 Est. 1845",
    5,
    {
      "NASA launches rockets from Cape Canaveral, Florida",
      "Walt Disney World is larger than San Francisco",
      "Florida has no state income tax",
      "More golf courses than any other US state",
      "The only place in the world where alligators and crocodiles coexist"
    }
  },

  // :28 — Texas (28th, Dec 29 1845)
  [28] = {
    "Texas",
    "Austin \xe2\x80\xa2 Est. 1845",
    5,
    {
      "Texas is so large that El Paso is closer to L.A. than to Houston",
      "Texas was its own country from 1836–1845",
      "Six flags have flown over Texas — more than any other US state",
      "The Alamo is in San Antonio",
      "Dell, AT&T, and ExxonMobil are all headquartered in Texas"
    }
  },

  // :29 — Iowa (29th, Dec 28 1846)
  [29] = {
    "Iowa",
    "Des Moines \xe2\x80\xa2 Est. 1846",
    5,
    {
      "Iowa produces 1/3 of the nation's corn supply",
      "First presidential caucus state every four years",
      "Iowa has more PhDs per capita than any other US state",
      "The first electronic digital computer (ENIAC) was designed in Iowa",
      "Iowa has more pigs than people — by a lot"
    }
  },

  // :30 — Wisconsin (30th, May 29 1848)
  [30] = {
    "Wisconsin",
    "Madison \xe2\x80\xa2 Est. 1848",
    5,
    {
      "Wisconsin produces more cheese than any other US state",
      "The Republican Party was founded in Ripon, Wisconsin in 1854",
      "Typewriter was invented in Milwaukee in 1868",
      "Green Bay Packers are the only fan-owned NFL franchise",
      "First kindergarten in the English-speaking world opened in Watertown"
    }
  },

  // :31 — California (31st, Sep 9 1850)
  [31] = {
    "California",
    "Sacramento \xe2\x80\xa2 Est. 1850",
    5,
    {
      "Largest state economy — would be 5th largest in the world",
      "Home to both the highest and lowest points in the contiguous US",
      "More Nobel Prize winners call California home than most countries",
      "Gold Rush of 1849 brought 300,000 settlers to California",
      "California produces 80% of the world's almonds"
    }
  },

  // :32 — Minnesota (32nd, May 11 1858)
  [32] = {
    "Minnesota",
    "St. Paul \xe2\x80\xa2 Est. 1858",
    5,
    {
      "Land of 10,000 Lakes — actually has over 11,000",
      "Mall of America in Bloomington is the largest in the US",
      "Post-it Notes were invented at 3M in St. Paul",
      "Minnesota has the most golfers per capita in the US",
      "Spam (the canned meat) was invented in Austin, Minnesota"
    }
  },

  // :33 — Oregon (33rd, Feb 14 1859)
  [33] = {
    "Oregon",
    "Salem \xe2\x80\xa2 Est. 1859",
    5,
    {
      "Crater Lake is the deepest lake in the US (1,943 ft)",
      "Oregon is the only state with a two-sided flag",
      "Powell's Books in Portland is the world's largest independent bookstore",
      "Oregon has the most ghost towns of any state",
      "Nike was founded in Eugene, Oregon in 1964"
    }
  },

  // :34 — Kansas (34th, Jan 29 1861)
  [34] = {
    "Kansas",
    "Topeka \xe2\x80\xa2 Est. 1861",
    5,
    {
      "Geographic center of the contiguous United States",
      "Produces enough wheat to feed everyone in the world for two weeks",
      "Pizza Hut was founded in Wichita in 1958",
      "Amelia Earhart was born in Atchison, Kansas",
      "Kansas has more miles of river than any other state"
    }
  },

  // :35 — West Virginia (35th, Jun 20 1863)
  [35] = {
    "West Virginia",
    "Charleston \xe2\x80\xa2 Est. 1863",
    5,
    {
      "Only state formed by seceding from another state (Virginia)",
      "Mother's Day was first celebrated in Grafton in 1908",
      "95% of the state is within the Appalachian mountain region",
      "Highest per-capita military enlistment rate in the US",
      "New River Gorge has the second-longest steel arch bridge in the world"
    }
  },

  // :36 — Nevada (36th, Oct 31 1864)
  [36] = {
    "Nevada",
    "Carson City \xe2\x80\xa2 Est. 1864",
    5,
    {
      "Las Vegas is the entertainment capital of the world",
      "Nevada is the driest state in the US",
      "Area 51 is in the Nevada desert",
      "Nevada has more mountain ranges than any other state except Alaska",
      "Most of the country's gold is mined in Nevada"
    }
  },

  // :37 — Nebraska (37th, Mar 1 1867)
  [37] = {
    "Nebraska",
    "Lincoln \xe2\x80\xa2 Est. 1867",
    5,
    {
      "The 911 emergency system was invented in Nebraska in 1967",
      "Warren Buffett was born in Omaha",
      "Nebraska is the only state with a unicameral legislature",
      "Kool-Aid was invented in Hastings, Nebraska in 1927",
      "Nebraska has more miles of river than any other state"
    }
  },

  // :38 — Colorado (38th, Aug 1 1876)
  [38] = {
    "Colorado",
    "Denver \xe2\x80\xa2 Est. 1876",
    5,
    {
      "Home to 53 mountains over 14,000 feet (called 'Fourteeners')",
      "Cheeseburger was invented in Denver in 1935",
      "Denver is exactly 1 mile above sea level",
      "Colorado has the highest average elevation of any US state",
      "Coors Brewery in Golden is the largest single brewery in the world"
    }
  },

  // :39 — North Dakota (39th, Nov 2 1889)
  [39] = {
    "North Dakota",
    "Bismarck \xe2\x80\xa2 Est. 1889",
    5,
    {
      "Least visited state in the US",
      "Produces more sunflowers than any other state",
      "North Dakota has never had a hurricane or earthquake",
      "The geographical center of North America is in Rugby, ND",
      "Has the lowest unemployment rate of any US state historically"
    }
  },

  // :40 — South Dakota (40th, Nov 2 1889)
  [40] = {
    "South Dakota",
    "Pierre \xe2\x80\xa2 Est. 1889",
    5,
    {
      "Mount Rushmore took 14 years to carve (1927–1941)",
      "Crazy Horse Memorial will be larger than Mount Rushmore",
      "Badlands National Park has the world's richest fossil beds",
      "Wall Drug started with free ice water signs in 1936",
      "South Dakota has no state income tax"
    }
  },

  // :41 — Montana (41st, Nov 8 1889)
  [41] = {
    "Montana",
    "Helena \xe2\x80\xa2 Est. 1889",
    5,
    {
      "Larger than Japan but has only 1 million people",
      "Glacier National Park spans the US-Canada border",
      "Montana has 3 times more cows than people",
      "Home to the largest grizzly bear population in the lower 48",
      "Little Bighorn Battlefield, site of Custer's Last Stand, is here"
    }
  },

  // :42 — Washington (42nd, Nov 11 1889)
  [42] = {
    "Washington",
    "Olympia \xe2\x80\xa2 Est. 1889",
    5,
    {
      "Microsoft, Amazon, and Boeing are all headquartered here",
      "Mt. Rainier is the most glaciated peak in the contiguous US",
      "Seattle gets less annual rain than New York City",
      "Starbucks was founded in Seattle in 1971",
      "Washington produces more apples than any other US state"
    }
  },

  // :43 — Idaho (43rd, Jul 3 1890)
  [43] = {
    "Idaho",
    "Boise \xe2\x80\xa2 Est. 1890",
    5,
    {
      "Produces 1/3 of all potatoes grown in the US",
      "Craters of the Moon looks like the surface of the Moon",
      "The world's first ski chairlift was built in Sun Valley in 1936",
      "Idaho has more whitewater rapids than any other state",
      "Shoshone Falls is taller than Niagara Falls"
    }
  },

  // :44 — Wyoming (44th, Jul 10 1890)
  [44] = {
    "Wyoming",
    "Cheyenne \xe2\x80\xa2 Est. 1890",
    5,
    {
      "Yellowstone was the world's first national park (1872)",
      "Least populous state in the US",
      "First state to grant women the right to vote (1869)",
      "Wyoming has the fewest traffic lights of any state",
      "Old Faithful erupts roughly every 90 minutes"
    }
  },

  // :45 — Utah (45th, Jan 4 1896)
  [45] = {
    "Utah",
    "Salt Lake City \xe2\x80\xa2 Est. 1896",
    5,
    {
      "Transcontinental Railroad was completed here at Promontory Summit",
      "Five national parks in one state: Zion, Bryce, Arches, Canyonlands, Capitol Reef",
      "The Great Salt Lake is saltier than the ocean",
      "Utah has the best snow on Earth — powder skiing mecca",
      "Jell-O is the official state snack food"
    }
  },

  // :46 — Oklahoma (46th, Nov 16 1907)
  [46] = {
    "Oklahoma",
    "Oklahoma City \xe2\x80\xa2 Est. 1907",
    5,
    {
      "Sits in 'Tornado Alley' — most tornadoes per square mile in the US",
      "The shopping cart was invented in Oklahoma City in 1937",
      "Route 66 'Mother Road' begins in Chicago and ends in Santa Monica via OK",
      "Oklahoma has more man-made lakes than any other state",
      "Parking meters were invented in Oklahoma City in 1935"
    }
  },

  // :47 — New Mexico (47th, Jan 6 1912)
  [47] = {
    "New Mexico",
    "Santa Fe \xe2\x80\xa2 Est. 1912",
    5,
    {
      "Santa Fe is the highest state capital in the US (7,000 ft)",
      "First atomic bomb was tested at Trinity Site in 1945",
      "White Sands is the world's largest gypsum dune field",
      "Roswell is famous for the alleged 1947 UFO crash",
      "New Mexico has more PhD scientists per capita than any other state"
    }
  },

  // :48 — Arizona (48th, Feb 14 1912)
  [48] = {
    "Arizona",
    "Phoenix \xe2\x80\xa2 Est. 1912",
    5,
    {
      "Grand Canyon is 1 mile deep and 277 miles long",
      "London Bridge was relocated to Lake Havasu City, Arizona",
      "Saguaro cactus only grows naturally in the Sonoran Desert",
      "Arizona is the only state that doesn't observe Daylight Saving Time (mostly)",
      "Meteor Crater is the best-preserved impact crater on Earth"
    }
  },

  // :49 — Alaska (49th, Jan 3 1959)
  [49] = {
    "Alaska",
    "Juneau \xe2\x80\xa2 Est. 1959",
    5,
    {
      "Largest state — bigger than the next 3 states combined",
      "Mt. Denali is the tallest mountain in North America (20,310 ft)",
      "Alaska has more coastline than all other states combined",
      "Some parts of Alaska get 24 hours of sunlight in summer",
      "The US bought Alaska from Russia for 2 cents per acre in 1867"
    }
  },

  // :50 — Hawaii (50th, Aug 21 1959)
  [50] = {
    "Hawaii",
    "Honolulu \xe2\x80\xa2 Est. 1959",
    5,
    {
      "Only state made up entirely of islands",
      "Only state that grows coffee commercially",
      "Hawaii is moving toward Japan at 4 inches per year",
      "The Hawaiian alphabet has only 13 letters",
      "Kilauea is one of the most active volcanoes on Earth"
    }
  },

  // :51 — Washington D.C.
  [51] = {
    "Washington D.C.",
    "Federal District \xe2\x80\xa2 Est. 1790",
    5,
    {
      "The only US city that isn't part of any state",
      "Residents only gained the right to vote for president in 1964",
      "The National Mall has 19 million visitors a year",
      "DC has more museums than any other city in the US",
      "The Capitol dome contains 8.9 million pounds of cast iron"
    }
  },

  // :52 — Puerto Rico
  [52] = {
    "Puerto Rico",
    "US Territory \xe2\x80\xa2 Est. 1898",
    5,
    {
      "3.2 million US citizens live in Puerto Rico",
      "San Juan is the oldest US city (1521)",
      "Puerto Ricans are US citizens but can't vote for president",
      "El Yunque is the only tropical rainforest in the US National Forest system",
      "Puerto Rico has won more Miss Universe titles than any other territory"
    }
  },

  // :53 — Guam
  [53] = {
    "Guam",
    "US Territory \xe2\x80\xa2 Est. 1898",
    5,
    {
      "Westernmost US territory — 'Where America's Day Begins'",
      "The brown tree snake wiped out most of Guam's native birds",
      "Guam was captured by Japan hours after Pearl Harbor",
      "Guam's Chamorro people have lived there for 4,000 years",
      "Guam is one of the most heavily militarized islands in the world"
    }
  },

  // :54 — US Virgin Islands
  [54] = {
    "US Virgin Islands",
    "US Territory \xe2\x80\xa2 Est. 1917",
    5,
    {
      "Purchased from Denmark for $25 million in 1917",
      "Drives on the left side of the road — unique in the US",
      "St. John has the most national park land of any US Virgin Island",
      "Has one of the world's deepest natural harbors (St. Thomas)",
      "Residents are US citizens but cannot vote in federal elections"
    }
  },

  // :55 — American Samoa
  [55] = {
    "American Samoa",
    "US Territory \xe2\x80\xa2 Est. 1900",
    5,
    {
      "Southernmost US territory — in the Southern Hemisphere",
      "The only US territory where residents are US nationals, not citizens",
      "Produces more NFL players per capita than anywhere in the world",
      "Home to one of the largest tuna canneries in the world",
      "National Park of American Samoa protects ancient rainforest"
    }
  },

  // :56 — Northern Mariana Islands
  [56] = {
    "N. Mariana Islands",
    "US Territory \xe2\x80\xa2 Est. 1978",
    4,
    {
      "Site of some of the most intense WWII Pacific battles",
      "Saipan was the launch point for the atomic bomb missions",
      "Only US territory with a casino gaming industry",
      "Marianas Trench nearby is the deepest point on Earth (36,000 ft)"
    }
  },

  // :57 — US Founding Facts
  [57] = {
    "Founding Facts",
    "USA \xe2\x80\xa2 Est. July 4, 1776",
    6,
    {
      "The Constitution has 27 amendments",
      "Only 39 of 55 delegates signed the Constitution",
      "The US flag has been redesigned 27 times",
      "Ben Franklin was 70 when he signed the Declaration",
      "John Adams and Thomas Jefferson both died on July 4, 1826",
      "George Washington's false teeth were made of ivory, not wood"
    }
  },

  // :58 — Records & Superlatives
  [58] = {
    "US Records",
    "Biggest, Longest, Most...",
    6,
    {
      "Mississippi River is the 4th longest river in the world",
      "US has the most tornadoes of any country (~1,200/year)",
      "Alaska has the most active volcanoes in the US (141)",
      "California's Death Valley holds the world heat record: 134°F",
      "The US has more public libraries than Starbucks",
      "America has won more Olympic medals than any other nation"
    }
  },

  // :59 — Fun/Weird US Facts
  [59] = {
    "Did You Know?",
    "Weird America",
    6,
    {
      "Montana has 3x more cattle than people",
      "There is a town in Texas called 'Ding Dong'",
      "Alaska has a town called 'Chicken' (population ~7)",
      "The US has no official language at the federal level",
      "Maine is the only state that borders exactly one other state",
      "Kansas is flatter than a pancake (scientifically proven)"
    }
  }

};
