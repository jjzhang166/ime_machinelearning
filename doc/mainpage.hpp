////////////////////////////////////////////////////////////
/// \mainpage
///
/// \section welcome Welcome
/// Welcome to the official IME Machine Learning documentation. Here you will
/// find a detailed view of all IMEML classes and functions.
///
/// \section Examples
/// Here is a short example of genetic algorithm:
///
/// \code
/// #include <stack>
/// #include <cmath>
///
/// #include "geneticalgorithm/valuegenome.h"
/// #include "geneticalgorithm/genalg.h"
///
/// class IntValueGenome : public ga::ValueGenome<int, true, true>
/// {
/// public:
///   IntValueGenome() {}
///   IntValueGenome(unsigned chromo_len) : ValueGenome(chromo_len, 0, 10) {}
/// };
///
/// int main(int argc, char** argv)
/// {
///   unsigned population_size = 10, generations = 10, elite = 2;
///   double mutation = 0.01, crossover = 0.7;
///
///   auto fitnessFunc = [] (IntValueGenome& g)
///   {
///     int t = 0;
///     auto x = g.extract();
///     for (unsigned i = 0; i < x.size(); ++i)
///       t += x[i];
///     return 1.0 / exp(abs(42 - t) / 10.f);
///   };
///
///   ga::GeneticAlgorithm<IntValueGenome> evolution {population_size, 6, elite,
///                                                   mutation, crossover,
///                                                   fitnessFunc};
///
///   for (unsigned i = 0; i < generations; ++i)
///   {
///     printf("generation %d:\n", evolution.generation());
///
///     auto population = evolution.population();
///     for (unsigned j = 0; j < population.size(); ++j)
///     {
///       printf("%.2f: ", population[j].getFitness());
///       auto chromo = population[j].extract();
///       for (unsigned k = 0; k < chromo.size(); ++k)
///         printf("%d ", chromo[k]);
///       printf("\n");
///     }
///     printf("\n");
///
///     evolution.epoch();
///   }
///
///   return 0;
/// }
/// \endcode
///
/// And another one for the graphical client:
/// \code
/// #include "client/gfx.h"
///
/// int main()
/// {
///   if(!gfx::initialize())
///     return 1;
///
///   if (!gfx::createWindow(640, 480, "Gfx Example"))
///     return 2;
///
///   gfx::setTime(0.0);
///   while (!gfx::windowShouldClose())
///   {
///     gfx::setCameraPosition(320 + 10 * gfx::getTime(), 240);
///
///     glPushMatrix();
///     glTranslatef(320.f, 240.f, 0.f);
///     glRotatef(100 * gfx::getTime(), 0.f, 0.f, 1.f);
///     glBegin(GL_TRIANGLES);
///     glColor3f(1.f, 0.f, 0.f); glVertex3f(  0.f, 107.f, 0.f);
///     glColor3f(0.f, 1.f, 0.f); glVertex3f(-92.f, -53.f, 0.f);
///     glColor3f(0.f, 0.f, 1.f); glVertex3f( 92.f, -53.f, 0.f);
///     glEnd();
///     glPopMatrix();
///
///     gfx::pollEvents();
///     gfx::swapBuffers();
///   }
///
///   gfx::terminate();
///
///   return 0;
/// }
/// \endcode
///
/// Another one, now showing physics and graphics:
/// \code
///
/// #include "client/client.h"
///
/// int main()
/// {
///   if (!gfx::initialize())
///     return 1;
///
///   if (!gfx::createWindow(640, 480, "Client Example"))
///     return 2;
///   gfx::setCameraPosition(0, 5);
///   gfx::setCameraHeight(10);
///
///   gfx::World world;
///   world.setGravity(0.f, -10.f);
///
///   gfx::Body body {world, 0, 8};
///   {
///     b2CircleShape circle;
///     circle.m_p.Set(0.f, 0.f);
///     circle.m_radius = 0.2f;
///     body.addFixture(&circle, 1.0f, 0.3f, 0.85f);
///   }
///
///   gfx::Body ground {world, 0, 0.5, 10};
///   ground.setStatic(true);
///   {
///     b2PolygonShape poly;
///     poly.SetAsBox(1.f, 0.5f);
///     ground.addShape(&poly);
///   }
///
///   gfx::setTime(0.0);
///   while (!gfx::windowShouldClose())
///   {
///     if (gfx::getTime() >= world.getTimeStep())
///     {
///       world.step();
///       gfx::setTime(0.0);
///     }
///
///     world.render();
///
///     gfx::pollEvents();
///     gfx::swapBuffers();
///   }
///
///   gfx::terminate();
/// }
/// \endcode
////////////////////////////////////////////////////////////
